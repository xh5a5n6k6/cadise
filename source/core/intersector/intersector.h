#pragma once

namespace cadise {

class Intersection;
class Ray;
class RGBColor;
class SurfaceInfo;
class Vector3;

// Intersector represents an object that is intersectable ( includes primitive and emiiter(area light) )
class Intersector {
public:
    virtual bool isIntersecting(Ray &ray, Intersection &intersection) = 0;
    virtual bool isOccluded(Ray &ray) = 0;
    virtual RGBColor emittance(Vector3 direction) = 0;

    virtual Vector3 evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) = 0;
    virtual Vector3 evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) = 0;
};

} // namespace cadise