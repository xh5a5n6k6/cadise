#include "Core/Estimator/AOV/AOEstimator.h"

#include "Core/Ray.h"
#include "Core/Scene.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Math/Random.h"
#include "Math/Warp/Hemisphere.h"

namespace cadise
{

AOEstimator::AOEstimator() :
    AOEstimator(4)
{}

AOEstimator::AOEstimator(const std::size_t numSampleRays) :
    AOVEstimator(),
    _numSampleRays(numSampleRays)
{}

void AOEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    Spectrum* const out_radiance) const
{
    CS_ASSERT(out_radiance);

    Spectrum            totalValue(0.0_r);
    Scene               localScene(scene);
    SurfaceIntersection si;
    Ray                 traceRay(ray);

    // HACK: manually unset environment light 
    localScene.unsetBackgroundSphere();

    if (!localScene.isIntersecting(traceRay, si))
    {
        out_radiance->set(1.0_r);

        return;
    }

    const Vector3R& V  = si.wi();
    const Vector3R& P  = si.surfaceDetail().position();
    const Vector3R& Ng = si.surfaceDetail().geometryNormal();

    for (std::size_t i = 0; i < _numSampleRays; ++i)
    {
        const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
        Vector3R L = Hemisphere::cosineWeightedSampling(sample);

        // transform L to world coordinate
        L = si.surfaceDetail().geometryLcs().localToWorld(L);
        L.normalizeLocal();
        if (V.dot(Ng) <= 0.0_r)
        {
            L.negateLocal();
        }

        traceRay.reset();
        traceRay.setOrigin(P);
        traceRay.setDirection(L);

        if (!localScene.isOccluded(traceRay))
        {
            totalValue.addLocal(1.0_r);
        }
    }

    totalValue.divLocal(static_cast<real>(_numSampleRays));

    out_radiance->set(totalValue);
}

} // namespace cadise