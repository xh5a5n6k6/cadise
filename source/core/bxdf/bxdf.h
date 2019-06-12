#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo;

class BxDF {
public:
    virtual Vector3R evaluate(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const = 0;
    virtual Vector3R evaluateSample(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const = 0;
};

} // namespace cadise