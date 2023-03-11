#pragma once

#include "Math/Type/MathType.h"

// forward declaration
namespace cadise
{
    class PrimitiveInfo;
    class Ray;
}

namespace cadise 
{

/*
    Intersector represents an object that is intersectable, 
    and it includes primitive, accelerator and transformedIntersector.
*/
class Intersector 
{
public:
    virtual ~Intersector();

    virtual void evaluateBound(AABB3R* const out_bound) const = 0;

    virtual bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const = 0;
    virtual bool isOccluded(const Ray& ray) const = 0;
};

} // namespace cadise