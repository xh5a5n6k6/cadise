#pragma once

#include "core/spectrum/spectrum.h"

#include <string>

namespace cadise {

class Ray;
class Scene;

class RadianceEstimator {
public:
    virtual ~RadianceEstimator();

    virtual void estimate(
        const Scene&    scene, 
        const Ray&      ray, 
        Spectrum* const out_radiance) const = 0;

    virtual std::string toString() const = 0;
};

} // namespace cadise