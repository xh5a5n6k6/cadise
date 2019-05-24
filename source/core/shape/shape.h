#pragma once

namespace cadise {

class Ray;
class SurfaceInfo;

class Shape {
public:
    virtual bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) = 0;
    virtual bool isOccluded(Ray &ray) = 0;
    virtual void sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface);
    virtual float area();
};

} // namespace cadise