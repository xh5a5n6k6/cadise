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
    WhittedIntegrator(int32 maxDepth);

    Spectrum traceRadiance(const Scene& scene, const Ray& ray) const override;

private:
    // TODO: refactor here, use for loop rather than this function
    //       to let ray doesn't need to store depth information
    Spectrum _radianceAtSpecularSurface(const Scene& scene, const Ray& ray, SurfaceIntersection& surfaceIntersection) const;

    int32 _maxDepth;
};

} // namespace cadise