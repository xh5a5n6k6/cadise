#pragma once

#include "core/integrator/integrator.h"

namespace cadise {

class SurfaceIntersection;

/*
    WhittedIntegrator means Whitted-style ray tracing integrator, it only
    consider direct lighting except hitting specular surface (support 
    specular reflection and specular refraction).
*/
class WhittedIntegrator : public Integrator {
public:
    explicit WhittedIntegrator(const int32 maxDepth);

    void traceRadiance(
        const Scene&    scene, 
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

private:
    int32 _maxDepth;
};

} // namespace cadise