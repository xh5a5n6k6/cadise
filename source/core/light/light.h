#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceGeometryInfo;

class Light {
public:
    virtual Vector3R evaluateSampleRadiance(Vector3R &lightDirection, const SurfaceGeometryInfo surfaceGeometryInfo, real &t, real &pdf) const = 0;
    virtual Vector3R position() const = 0;
    virtual Vector3R color() const = 0;
};

} // namespace cadise