#pragma once

#include "core/integrator/integrator.h"

namespace cadise {

class SurfaceIntersection;

/*
    WhittedIntegrator is Whitted-style ray tracing integrator, 
    it considers direct lighting and reflection/refraction effects.
    (reflection/refraction effects are calculated only at specular surface)
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