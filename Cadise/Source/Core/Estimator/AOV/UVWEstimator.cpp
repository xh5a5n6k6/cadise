#include "Core/Estimator/AOV/UVWEstimator.h"

#include "Core/Ray.h"
#include "Core/Scene.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Math/Math.h"

namespace cadise
{

void UVWEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    Spectrum* const out_radiance) const
{
    CS_ASSERT(out_radiance);

    Scene               localScene(scene);
    SurfaceIntersection si;
    Ray                 traceRay(ray);

    // HACK: manually unset environment light 
    localScene.unsetBackgroundSphere();

    if (!localScene.isIntersecting(traceRay, si))
    {
        out_radiance->set(0.0_r);
    }
    else
    {
        const Vector3R& uvw = si.surfaceDetail().uvw();

        out_radiance->set({
            math::fractional(uvw.x()),
            math::fractional(uvw.y()),
            math::fractional(uvw.z()) });
    }
}

} // namespace cadise