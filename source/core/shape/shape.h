#pragma once

#include "math/aabb.h"

namespace cadise {

class Ray;
class SurfaceInfo;

class Shape {
public:
    virtual AABB3R bound() const = 0;

    virtual bool isIntersecting(Ray &ray, SurfaceInfo &surfaceInfo) const = 0;
    virtual bool isOccluded(Ray &ray) const = 0;

    virtual void sampleSurface(const SurfaceInfo inSurface, SurfaceInfo &outSurface) const;
    virtual real samplePdfA() const;

    virtual real area() const;
};

} // namespace cadise