#include "Core/Estimator/VPTEstimator.h"

#include "Core/Gear/RussianRoulette.h"
#include "Core/Gear/Sample/bsdfSample.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/AreaLight.h"
#include "Core/Ray.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"

namespace cadise
{

VPTEstimator::VPTEstimator(const int32 maxDepth) :
    RadianceEstimator(),
    _maxDepth(maxDepth)
{
    CS_ASSERT_GE(maxDepth, 0);
}

void VPTEstimator::estimate(
    const Scene&    scene, 
    const Ray&      ray,
    Spectrum* const out_radiance) const
{
    CS_ASSERT(out_radiance);

    const TransportInfo transportInfo(ETransportMode::Radiance);
    
    Spectrum totalRadiance(0.0_r);
    Spectrum pathThroughput(1.0_r);
    Ray      traceRay(ray);

    for (int32 bounceTimes = 0; bounceTimes < _maxDepth; ++bounceTimes)
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

        if (primitive->areaLight()) 
        {
            const Spectrum emittance = primitive->areaLight()->emittance(intersection);

            totalRadiance.addLocal(pathThroughput.mul(emittance));
        }

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

        pathThroughput.mulLocal(reflectance.mul(LdotN / pdfW));

        // use russian roulette to decide if the ray needs to be kept tracking
        if (bounceTimes > 2) 
        {
            Spectrum newPathThroughput;
            if (!RussianRoulette::isSurvivedOnNextRound(pathThroughput, &newPathThroughput))
            {
                break;
            }

            pathThroughput = newPathThroughput;
        }

        if (pathThroughput.isZero())
        {
            break;
        }

        traceRay.reset();
        traceRay.setOrigin(P);
        traceRay.setDirection(L);
    }
    
    out_radiance->set(totalRadiance);
}

} // namespace cadise