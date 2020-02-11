#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class Ray;
class Scene;

class Integrator {
public:
    virtual void traceRadiance(
        const Scene&    scene, 
        const Ray&      ray, 
        Spectrum* const out_radiance) const = 0;
};

} // namespace cadise