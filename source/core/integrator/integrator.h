#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class Ray;
class Scene;

class Integrator {
public:
    virtual Spectrum traceRadiance(const Scene& scene, const Ray& ray) const = 0;
};

} // namespace cadise