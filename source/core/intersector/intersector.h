#pragma once

#include "math/aabb.h"

namespace cadise {

class Intersection;
class Ray;
class RGBColor;
class SurfaceInfo;

// Intersector represents an object that is intersectable, 
// and it includes primitive, emiiter(area light), and accelerator.
class Intersector {
public:
    virtual AABB3R bound() const = 0;

    virtual bool isIntersecting(Ray &ray, Intersection &intersection) const = 0;
    virtual bool isOccluded(Ray &ray) const = 0;

    virtual RGBColor emittance(const Vector3R direction) const = 0;

    virtual Vector3R evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const = 0;
    virtual Vector3R evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const = 0;
};

} // namespace cadise