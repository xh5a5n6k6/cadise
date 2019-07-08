#pragma once

#include "math/aabb.h"

namespace cadise {

class Ray;
class PrimitiveInfo;

// Intersector represents an object that is intersectable, 
// and it includes primitive, accelerator and transformedIntersector.
class Intersector {
public:
    virtual AABB3R bound() const = 0;

    virtual bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const = 0;
    virtual bool isOccluded(Ray& ray) const = 0;
};

} // namespace cadise