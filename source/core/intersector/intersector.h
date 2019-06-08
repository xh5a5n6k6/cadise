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
    virtual AABB3R bound() = 0;

    virtual bool isIntersecting(Ray &ray, Intersection &intersection) = 0;
    virtual bool isOccluded(Ray &ray) = 0;

    virtual RGBColor emittance(Vector3R direction) = 0;

    virtual Vector3R evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) = 0;
    virtual Vector3R evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) = 0;
};

} // namespace cadise