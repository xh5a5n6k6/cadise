#include "core/estimator/aov/uvwEstimator.h"

#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/math.h"

namespace cadise
{

void UvwEstimator::estimate(
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