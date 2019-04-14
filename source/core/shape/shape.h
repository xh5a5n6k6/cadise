#pragma once

namespace cadise {

class Ray;
class SurfaceInfo;

class Shape {
public:
    virtual bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) = 0;
    virtual bool isOccluded(Ray &ray) = 0;
};

} // namespace cadise