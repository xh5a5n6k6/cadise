#pragma once

#include "core/intersector.h"

namespace cadise {

class Accelerator : public Intersector {
public:
    virtual bool isIntersecting(Ray &ray, Intersection &intersection) override = 0;
    virtual bool isOccluded(Ray &ray) override = 0;
    virtual RGBColor emittance() override;
};

} // namespace cadise