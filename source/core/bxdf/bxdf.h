#pragma once

#include "core/surfaceInfo.h"

namespace cadise {

class BxDF {
public:
    virtual Vector3 evaluate(Vector3 &inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) = 0;
    virtual void evaluateSample(Vector3 &inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo){}
};

} // namespace cadise