#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo;

class BxDF {
public:
    virtual Vector3R evaluate(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) = 0;
    virtual Vector3R evaluateSample(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) = 0;
};

} // namespace cadise