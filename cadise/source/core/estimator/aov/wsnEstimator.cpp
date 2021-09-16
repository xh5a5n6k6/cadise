#include "core/estimator/aov/wsnEstimator.h"

#include "core/ray.h"
#include "core/scene.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

namespace cadise 
{

void WsnEstimator::estimate(
    const Scene&    scene,
    const Ray&      ray,
    Spectrum* const out_radiance) const 
{
    CADISE_ASSERT(out_radiance);

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
        const Vector3R& Ns = si.surfaceDetail().shadingNormal();

        out_radiance->set(Spectrum(Ns.mul(0.5_r).add(0.5_r)));
    }
}

} // namespace cadise