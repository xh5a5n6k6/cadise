#pragma once

#include "core/intersector/intersector.h"

namespace cadise {

class Accelerator : public Intersector {
public:
    virtual AABB3R bound() const override;

    virtual bool isIntersecting(Ray &ray, Intersection &intersection) const override = 0;
    virtual bool isOccluded(Ray &ray) const override = 0;

    virtual RGBColor emittance(const Vector3R direction) const override;

    virtual Vector3R evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const override;
    virtual Vector3R evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo, real &pdf) const override;
};

} // namespace cadise