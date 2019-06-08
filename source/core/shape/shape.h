#pragma once

#include "math/aabb.h"

namespace cadise {

class Ray;
class SurfaceInfo;

class Shape {
public:
    virtual AABB3R bound() = 0;

    virtual bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) = 0;
    virtual bool isOccluded(Ray &ray) = 0;

    virtual void sampleSurface(SurfaceInfo inSurface, SurfaceInfo &outSurface);
    virtual real samplePdfA();

    virtual real area();
};

} // namespace cadise