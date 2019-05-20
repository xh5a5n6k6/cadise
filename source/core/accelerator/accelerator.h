#pragma once

#include "core/intersector/intersector.h"

namespace cadise {

class Accelerator : public Intersector {
public:
    virtual bool isIntersecting(Ray &ray, Intersection &intersection) override = 0;
    virtual bool isOccluded(Ray &ray) override = 0;
    virtual RGBColor emittance(Vector3 direction) override;

    virtual Vector3 evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) override;
    virtual Vector3 evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;
};

} // namespace cadise