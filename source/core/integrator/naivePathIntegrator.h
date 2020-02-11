#pragma once

#include "core/integrator/integrator.h"

namespace cadise {

/*
     NaivePathIntegrator means original path integrator, it means a path 
     has contribution only when it hits an emitter. It may be the most
     fundamental global illumination technique but still unbiased, so it 
     is a good reference for ground truth rendering.
*/
class NaivePathIntegrator : public Integrator {
public:
    NaivePathIntegrator(const int32 maxDepth);

    void traceRadiance(
        const Scene&    scene, 
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

private:
    int32 _maxDepth;
};

} // namespace cadise