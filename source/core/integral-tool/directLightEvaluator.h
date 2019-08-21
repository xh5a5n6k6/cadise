#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class Bsdf;
class Light;
class Scene;
class SurfaceIntersection;

class DirectLightEvaluator {
public:
    static Spectrum evaluate(const Scene& scene, const SurfaceIntersection& surfaceIntersection,
                             const Bsdf* bsdf, const Light* light);
};

} // namespace cadise