#include "core/renderer/render-work/photonMapWork.h"

#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/bsdfSample.h"
#include "core/integral-tool/sample/emitLightSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/renderer/photon-mapping/photon.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

#include <limits>

namespace cadise {

PhotonMapWork::PhotonMapWork(
    const Scene* const         scene,
    std::vector<Photon>* const photons,
    std::size_t* const         numPhotonPaths) :

    RenderWork(),
    _scene(scene),
    _photons(photons),
    _numPhotonPaths(numPhotonPaths),
    _maxNumPhotons(std::numeric_limits<std::size_t>::max()),
    _maxNumPhotonPaths(std::numeric_limits<std::size_t>::max()) {

    CADISE_ASSERT(scene);
    CADISE_ASSERT(photons);
    CADISE_ASSERT(numPhotonPaths);
}

void PhotonMapWork::work() const {
    CADISE_ASSERT(
        !(_maxNumPhotons     == std::numeric_limits<std::size_t>::max() &&
          _maxNumPhotonPaths == std::numeric_limits<std::size_t>::max()));

    std::size_t numPhotonPaths = 0;
    std::size_t numPhotons     = 0;
    while (numPhotons < _maxNumPhotons && numPhotonPaths < _maxNumPhotonPaths) {
        ++numPhotonPaths;

        real pickLightPdf;
        const Light* sampleLight = _scene->sampleOneLight(&pickLightPdf);

        CADISE_ASSERT(sampleLight);

        EmitLightSample emitLightSample;
        sampleLight->evaluateEmitSample(&emitLightSample);
        if (!emitLightSample.isValid()) {
            continue;
        }

        const Spectrum& emittance     = emitLightSample.radiance();
        const Vector3R& emitPosition  = emitLightSample.emitPosition();
        const Vector3R& emitDirection = emitLightSample.emitDirection();
        const Vector3R& emitN         = emitLightSample.emitNormal();
        const real      emitPdfA      = emitLightSample.pdfA();
        const real      emitPdfW      = emitLightSample.pdfW();

        const real numerator   = emitDirection.absDot(emitN);
        const real denominator = pickLightPdf * emitPdfA * emitPdfW;

        Spectrum      throughputRadiance(emittance.mul(numerator / denominator));
        TransportInfo transportInfo(ETransportMode::IMPORTANCE);
        Ray           traceRay(emitPosition, emitDirection);

        // tracing light ray
        for(int32 bounceTimes = 0;; ++bounceTimes) {
            SurfaceIntersection intersection;
            if (!_scene->isIntersecting(traceRay, intersection)) {
                break;
            }

            const Primitive* primitive = intersection.primitiveInfo().primitive();
            const Bsdf*      bsdf      = primitive->bsdf();

            const Vector3R& P  = intersection.surfaceDetail().position();
            const Vector3R& Ns = intersection.surfaceDetail().shadingNormal();

            // only store photon at non-specular surface
            if (bsdf->lobes().hasAtLeastOne({
                ELobe::DIFFUSE_REFLECTION,
                ELobe::DIFFUSE_TRANSMISSION,
                ELobe::GLOSSY_REFLECTION,
                ELobe::GLOSSY_TRANSMISSION })) {

                Photon photon;
                photon.setPosition(P);
                photon.setFromDirection(traceRay.direction().negate());
                photon.setThroughputRadiance(throughputRadiance);

                _photons->push_back(std::move(photon));

                ++numPhotons;
                if (numPhotons == _maxNumPhotons) {
                    break;
                }
            }

            // keep tracing with bsdf sampling
            BsdfSample bsdfSample;
            bsdf->evaluateSample(transportInfo, intersection, &bsdfSample);
            if (!bsdfSample.isValid()) {
                break;
            }

            const Spectrum& reflectance = bsdfSample.scatterValue();
            const Vector3R& L           = bsdfSample.scatterDirection();
            const real      pdfW        = bsdfSample.pdfW();
            const real      LdotN       = L.absDot(Ns);

            throughputRadiance.mulLocal(reflectance.mul(LdotN / pdfW));
            // TODO: for non-symmetric scattering correction
            //       when using shading normal
            throughputRadiance.mulLocal(1.0_r);

            // use russian roulette to decide if the ray needs to be kept tracking
            Spectrum newThroughputRadiance;
            if (!RussianRoulette::isSurvivedOnNextRound(throughputRadiance, &newThroughputRadiance)) {
                break;
            }
            else {
                throughputRadiance = newThroughputRadiance;
            }

            if (throughputRadiance.isZero()) {
                break;
            }

            traceRay.reset();
            traceRay.setOrigin(P);
            traceRay.setDirection(L);
        }
    } // end while loop

    *_numPhotonPaths = numPhotonPaths;
}

void PhotonMapWork::setMaxNumPhotons(const std::size_t maxNumPhotons) {
    _maxNumPhotons = maxNumPhotons;
}

void PhotonMapWork::setMaxNumPhotonPaths(const std::size_t maxNumPhotonPaths) {
    _maxNumPhotonPaths = maxNumPhotonPaths;
}

} // namespace cadise