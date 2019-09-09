#pragma once

#include "core/integrator/integrator.h"

namespace cadise {

// PurePathIntegrator means original path integrator, it means a path 
// has contribution only when it hits an emitter. It may be the most
// fundamental global illumination technique but still unbiased, so it 
// is a good reference for ground truth rendering.
//
class PurePathIntegrator : public Integrator {
public:
    PurePathIntegrator(const int32 maxDepth);

    Spectrum traceRadiance(const Scene& scene, const Ray& ray) const override;

private:
    int32 _maxDepth;
};

} // namespace cadise