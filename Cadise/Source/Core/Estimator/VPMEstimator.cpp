#include "Core/Estimator/VPMEstimator.h"

#include "Core/Gear/RussianRoulette.h"
#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/AreaLight.h"
#include "Core/Ray.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"

#include <vector>

namespace cadise 
{

VPMEstimator::VPMEstimator(
    const PhotonMap* const photonMap,
    const std::size_t      numPhotonPaths,
    const real             searchRadius) :

    _photonMap(photonMap),
    _searchRadius(searchRadius) 
{
    CS_ASSERT(photonMap);
    CS_ASSERT_GT(numPhotonPaths, 0);
    CS_ASSERT_GT(searchRadius, 0.0_r);

    _kernelFactor = 1.0_r / (constant::pi<real> * _searchRadius * _searchRadius * numPhotonPaths);
}

void VPMEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    Spectrum* const out_radiance) const 
{
    CS_ASSERT(out_radiance);

    const TransportInfo transportInfo(ETransportMode::Importance);
    Spectrum totalRadiance(0.0_r);
    Spectrum pathThroughput(1.0_r);
    Ray      traceRay(ray);

    for (int32 bounceTimes = 0;; ++bounceTimes) 
    {
        SurfaceIntersection intersection;
        if (!scene.isIntersecting(traceRay, intersection)) 
        {
            break;
        }

        const Primitive* primitive = intersection.primitiveInfo().primitive();
        const BSDF*      bsdf      = primitive->bsdf();

        const Vector3R& P  = intersection.surfaceDetail().position();
        const Vector3R& Ns = intersection.surfaceDetail().shadingNormal();

        const bool isSpecular = bsdf->lobes().hasAtLeastOne({
            ELobe::SpecularReflection,
            ELobe::SpecularTransmission });

        // add radiance if hitting area light
        if (primitive->areaLight()) 
        {
            const Spectrum emittance = primitive->areaLight()->emittance(intersection);

            totalRadiance.addLocal(pathThroughput.mul(emittance));
        }

        // collect photons near the non-specular surface,
        // then use them to do radiance estimation
        if (!isSpecular)
        {
            Spectrum radiance(0.0_r);

            std::vector<Photon> nearPhotons;
            _photonMap->findWithRange(P, _searchRadius, &nearPhotons);

            for (const auto& photon : nearPhotons) 
            {
                const Vector3R& fromDirection      = photon.fromDirection();
                const Spectrum& throughputRadiance = photon.throughputRadiance();

                intersection.setWo(fromDirection);

                Spectrum reflectance = bsdf->evaluate(transportInfo, intersection.reverse());
                if (reflectance.isZero())
                {
                    continue;
                }

                // TODO:
                // for non-symmetric scattering correction
                // when using shading normal
                reflectance.mulLocal(1.0_r);

                radiance.addLocal(pathThroughput.mul(reflectance.mul(throughputRadiance)));
            }

            totalRadiance.addLocal(radiance.mul(_kernelFactor));

            // once hitting non-specular surface, we
            // use near photons to estimate radiance and then exit
            break;
        }

        // only trace next ray at specular surface
        // according to bsdf sampling
        else 
        {
            // TODO: sample two directions if encountering
            //       dielectric bsdf
            BSDFSample bsdfSample;
            bsdf->evaluateSample(TransportInfo(), intersection, &bsdfSample);
            if (!bsdfSample.isValid()) 
            {
                break;
            }

            const Spectrum& reflectance = bsdfSample.scatterValue();
            const Vector3R& L           = bsdfSample.scatterDirection();
            const real      pdfW        = bsdfSample.pdfW();
            const real      LdotN       = L.absDot(Ns);

            pathThroughput.mulLocal(reflectance.mul(LdotN / pdfW));
            if (pathThroughput.isZero()) 
            {
                break;
            }

            traceRay.reset();
            traceRay.setOrigin(P);
            traceRay.setDirection(L);
        }
    }

    out_radiance->set(totalRadiance);
}

} // namespace cadise