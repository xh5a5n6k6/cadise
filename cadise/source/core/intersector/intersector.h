#pragma once

#include "math/type/mathType.h"

namespace cadise { class PrimitiveInfo; }
namespace cadise { class Ray; }

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