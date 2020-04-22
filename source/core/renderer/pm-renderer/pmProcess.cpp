#include "core/renderer/pm-renderer/pmProcess.h"

#include "core/integral-tool/russianRoulette.h"
#include "core/integral-tool/sample/bsdfSample.h"
#include "core/integral-tool/sample/emitLightSample.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/renderer/pm-renderer/photon.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

namespace cadise {

PmProcess::PmProcess(const Scene* const scene) :
    _scene(scene) {

    CADISE_ASSERT(scene);
}

void PmProcess::process(
    const std::size_t          maxNumPhotons,
    std::vector<Photon>* const out_photons,
    std::size_t* const         out_numPhotonPaths) const {

    CADISE_ASSERT(out_photons);
    CADISE_ASSERT(out_numPhotonPaths);

    std::size_t numPhotonPaths = 0;
    std::size_t numPhotons     = 0;
    while (numPhotons < maxNumPhotons) {
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
        Spectrum throughputRadiance(emittance * (numerator / denominator));
        Ray      traceRay(emitPosition, emitDirection);

        // tracing light ray
        for(int32 bounceTimes = 0;; ++bounceTimes) {
            SurfaceIntersection intersection;
            if (!_scene->isIntersecting(traceRay, intersection)) {
                break;
            }

            const Primitive* primitive = intersection.primitiveInfo().primitive();
            const Bsdf*      bsdf      = primitive->bsdf();

            const Vector3R& P  = intersection.surfaceInfo().position();
            const Vector3R& Ns = intersection.surfaceInfo().shadingNormal();

            // only store photon at non-specular surface
            if (bsdf->type().hasAtLeastOne(BxdfType::DIFFUSE_REFLECTION,
                                           BxdfType::DIFFUSE_TRANSMISSION,
                                           BxdfType::GLOSSY_REFLECTION,
                                           BxdfType::GLOSSY_TRANSMISSION)) {

                Photon photon;
                photon.setPosition(P);
                photon.setInDirection(traceRay.direction().reverse());
                photon.setThroughputRadiance(throughputRadiance);

                out_photons->push_back(std::move(photon));

                ++numPhotons;
                if (numPhotons == maxNumPhotons) {
                    break;
                }
            }

            // keep tracing with bsdf sampling
            BsdfSample bsdfSample;
            bsdf->evaluateSample(TransportInfo(), intersection, &bsdfSample);
            if (!bsdfSample.isValid()) {
                break;
            }

            const Spectrum& reflectance = bsdfSample.scatterValue();
            const Vector3R& L           = bsdfSample.scatterDirection();
            const real      pdfW        = bsdfSample.pdfW();
            const real      LdotN       = L.absDot(Ns);

            throughputRadiance *= reflectance * LdotN / pdfW;
            // TODO: for non-symmetric scattering correction
            //       when using shading normal
            throughputRadiance *= 1.0_r;

            // use russian roulette to decide if the ray needs to be kept tracking
            Spectrum newThroughput;
            if (!RussianRoulette::isSurvivedOnNextRound(throughputRadiance, &newThroughput)) {
                break;
            }

            throughputRadiance = newThroughput;

            if (throughputRadiance.isZero()) {
                break;
            }

            traceRay.reset();
            traceRay.setOrigin(P);
            traceRay.setDirection(L);
        }
    } // end while loop

    *out_numPhotonPaths = numPhotonPaths;
}

} // namespace cadise