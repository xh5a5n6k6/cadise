#pragma once

#include "core/integrator/integrator.h"

#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceIntersection;

// WhittedIntegrator means Whitted-style ray tracing integrator, it only
// consider direct lighting except hitting specular surface (support 
// specular reflection and specular refraction).
//
class WhittedIntegrator : public Integrator {
public:
    WhittedIntegrator(int32 maxDepth);

    Spectrum traceRadiance(const Scene& scene, Ray& ray) const override;

private:
    Spectrum _radianceAtSpecularSurface(const Scene& scene, Ray& ray, SurfaceIntersection& surfaceIntersection) const;

    int32 _maxDepth;
};

} // namespace cadise