#include "Core/Estimator/AOV/WSNEstimator.h"

#include "Core/Ray.h"
#include "Core/Scene.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"

namespace cadise
{

void WSNEstimator::estimate(
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
        const Vector3R& Ns = si.surfaceDetail().shadingNormal();

        out_radiance->set(Spectrum(Ns.mul(0.5_r).add(0.5_r)));
    }
}

} // namespace cadise