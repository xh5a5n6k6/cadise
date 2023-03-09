#pragma once

#include "core/intersector/primitive/primitive.h"

namespace cadise 
{

class InfiniteSphere : public Primitive 
{
public:
    InfiniteSphere();
    explicit InfiniteSphere(const real radius);

    void evaluateBound(AABB3R* const out_bound) const override;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override;
    bool isOccluded(const Ray& ray) const override;

    void evaluateSurfaceDetail(
        const PrimitiveInfo& primitiveInfo, 
        SurfaceDetail* const out_surface) const override;

    void uvwToPosition(
        const Vector3R& uvw, 
        Vector3R* const out_position) const override;

private:
    real _radius;
};

} // namespace cadise