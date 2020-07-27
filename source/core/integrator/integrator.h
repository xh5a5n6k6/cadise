#pragma once

#include "core/spectrum/spectrum.h"

#include <string>

namespace cadise {

class Ray;
class Scene;

class Integrator {
public:
    virtual ~Integrator();

    virtual void traceRadiance(
        const Scene&    scene, 
        const Ray&      ray, 
        Spectrum* const out_radiance) const = 0;

    virtual std::string toString() const;
};

// header implementation

inline std::string Integrator::toString() const {
    return "Integrator";
}

} // namespace cadise