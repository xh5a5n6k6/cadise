#pragma once

#include "core/integrator/integrator.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceIntersection;

// WhittedIntegrator is Whitted-style ray tracing integrator
class WhittedIntegrator : public Integrator {
public:
    WhittedIntegrator(int32 maxDepth);

    Spectrum traceRadiance(const Scene& scene, Ray& ray) const override;

private:
    Spectrum _radianceOnScattering(const Scene& scene, Ray& ray, SurfaceIntersection& surfaceIntersection) const;

    int32 _maxDepth;
};

} // namespace cadise