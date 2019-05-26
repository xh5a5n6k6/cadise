#pragma once

#include "math/vector.h"

namespace cadise {

class Intersection;
class Ray;
class RGBColor;
class SurfaceInfo;

// Intersector represents an object that is intersectable, 
// and it includes primitive and emiiter(area light).
class Intersector {
public:
    virtual bool isIntersecting(Ray &ray, Intersection &intersection) = 0;
    virtual bool isOccluded(Ray &ray) = 0;
    virtual RGBColor emittance(Vector3F direction) = 0;

    virtual Vector3F evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) = 0;
    virtual Vector3F evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) = 0;
};

} // namespace cadise