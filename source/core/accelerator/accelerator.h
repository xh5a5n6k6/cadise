#pragma once

#include "core/intersector/intersector.h"

namespace cadise {

class Accelerator : public Intersector {
public:
    virtual AABB3R bound() override;

    virtual bool isIntersecting(Ray &ray, Intersection &intersection) override = 0;
    virtual bool isOccluded(Ray &ray) override = 0;

    virtual RGBColor emittance(Vector3R direction) override;

    virtual Vector3R evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) override;
    virtual Vector3R evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) override;
};

} // namespace cadise