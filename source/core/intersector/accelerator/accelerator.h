#pragma once

#include "core/intersector/intersector.h"

namespace cadise {

class Accelerator : public Intersector {
public:
    virtual AABB3R bound() const override;

    virtual bool isIntersecting(Ray& ray, PrimitiveInfo& primitiveInfo) const override = 0;
    virtual bool isOccluded(Ray& ray) const override = 0;
};

} // namespace cadise