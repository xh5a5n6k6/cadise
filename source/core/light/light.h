#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo;

class Light {
public:
    virtual Vector3F evaluateSampleRadiance(Vector3F &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) = 0;
    virtual Vector3F position() = 0;
    virtual Vector3F color() = 0;
};

} // namespace cadise