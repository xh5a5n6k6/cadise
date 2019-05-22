#pragma once

namespace cadise {

class SurfaceInfo;
class Vector3;

class Light {
public:
    virtual Vector3 evaluateSampleRadiance(Vector3 &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) = 0;
    virtual Vector3 position() = 0;
    virtual Vector3 color() = 0;
};

} // namespace cadise