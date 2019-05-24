#pragma once

#include "math/vector.h"

namespace cadise {

class SurfaceInfo;

class BxDF {
public:
    virtual Vector3F evaluate(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) = 0;
    virtual Vector3F evaluateSample(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) = 0;
};

} // namespace cadise