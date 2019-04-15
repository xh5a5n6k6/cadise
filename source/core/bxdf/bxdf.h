#pragma once

namespace cadise {

class SurfaceInfo;
class Vector3;

class BxDF {
public:
    virtual Vector3 evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) = 0;
    virtual Vector3 evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) = 0;
};

} // namespace cadise