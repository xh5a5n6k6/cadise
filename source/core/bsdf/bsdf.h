#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceIntersection;

class BSDF {
public:
    virtual Vector3R evaluate(const SurfaceIntersection& surfaceIntersection) const = 0;
    virtual Vector3R evaluateSample(SurfaceIntersection& surfaceIntersection) const = 0;
};

} // namespace cadise