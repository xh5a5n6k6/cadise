#pragma once

#include "core/intersector/intersector.h"

namespace cadise {

class Accelerator : public Intersector {
public:
    void evaluateBound(AABB3R* const out_bound) const override = 0;

    bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override = 0;
    bool isOccluded(const Ray& ray) const override = 0;
};

} // namespace cadise