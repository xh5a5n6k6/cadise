#include "Core/Renderer/RenderWork/PhotonMapWork.h"

#include "Core/Gear/RussianRoulette.h"
#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Gear/Sample/EmitLightSample.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/Light.h"
#include "Core/Ray.h"
#include "Core/Renderer/PhotonMapping/Photon.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"

#include <limits>

namespace cadise 
{

PhotonMapWork::PhotonMapWork(
    const Scene* const         scene,
    std::vector<Photon>* const photons,
    std::size_t* const         numPhotonPaths) :

    RenderWork(),
    _scene(scene),
    _photons(photons),
    _numPhotonPaths(numPhotonPaths),
    _maxNumPhotons(std::numeric_limits<std::size_t>::max()),
    _maxNumPhotonPaths(std::numeric_limits<std::size_t>::max()) 
{
    CS_ASSERT(scene);
    CS_ASSERT(photons);
    CS_ASSERT(numPhotonPaths);
}

void PhotonMapWork::work() const 
{
    CS_ASSERT(
        !(_maxNumPhotons     == std::numeric_limits<std::size_t>::max() &&
          _maxNumPhotonPaths == std::numeric_limits<std::size_t>::max()));

    std::size_t numPhotonPaths = 0;
    std::size_t numPhotons     = 0;
    while (numPhotons < _maxNumPhotons && numPhotonPaths < _maxNumPhotonPaths) 
    {
        ++numPhotonPaths;

        real pickLightPdf;
        const Light* sampleLight = _scene->sampleOneLight(&pickLightPdf);

        CS_ASSERT(sampleLight);

        EmitLightSample emitLightSample;
        sampleLight->evaluateEmitSample(&emitLightSample);
        if (!emitLightSample.isValid()) 
        {
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
        TransportInfo transportInfo(ETransportMode::Importance);
        Ray           traceRay(emitPosition, emitDirection);

        // tracing light ray
        for(int32 bounceTimes = 0;; ++bounceTimes) 
        {
            SurfaceIntersection intersection;
            if (!_scene->isIntersecting(traceRay, intersection))
            {
                break;
            }

            const Primitive* primitive = intersection.primitiveInfo().primitive();
            const BSDF*      bsdf      = primitive->bsdf();

            const Vector3R& P  = intersection.surfaceDetail().position();
            const Vector3R& Ns = intersection.surfaceDetail().shadingNormal();

            // only store photon at non-specular surface
            if (bsdf->lobes().hasAtLeastOne({
                ELobe::DiffuseReflection,
                ELobe::DiffuseTransmission,
                ELobe::GlossyReflection,
                ELobe::GlossyTransmission })) 
            {
                Photon photon;
                photon.setPosition(P);
                photon.setFromDirection(traceRay.direction().negate());
                photon.setThroughputRadiance(throughputRadiance);

                _photons->push_back(std::move(photon));

                ++numPhotons;
                if (numPhotons == _maxNumPhotons) 
                {
                    break;
                }
            }

            // keep tracing with bsdf sampling
            BSDFSample bsdfSample;
            bsdf->evaluateSample(transportInfo, intersection, &bsdfSample);
            if (!bsdfSample.isValid())
            {
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
            if (!RussianRoulette::isSurvivedOnNextRound(throughputRadiance, &newThroughputRadiance)) 
            {
                break;
            }
            else
            {
                throughputRadiance = newThroughputRadiance;
            }

            if (throughputRadiance.isZero())
            {
                break;
            }

            traceRay.reset();
            traceRay.setOrigin(P);
            traceRay.setDirection(L);
        }
    } // end while loop

    *_numPhotonPaths = numPhotonPaths;
}

void PhotonMapWork::setMaxNumPhotons(const std::size_t maxNumPhotons) 
{
    _maxNumPhotons = maxNumPhotons;
}

void PhotonMapWork::setMaxNumPhotonPaths(const std::size_t maxNumPhotonPaths)
{
    _maxNumPhotonPaths = maxNumPhotonPaths;
}

} // namespace cadise