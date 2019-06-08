#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo;

class Light {
public:
    virtual Vector3R evaluateSampleRadiance(Vector3R &lightDirection, SurfaceInfo &surfaceInfo, real &t, real &pdf) = 0;
    virtual Vector3R position() = 0;
    virtual Vector3R color() = 0;
};

} // namespace cadise