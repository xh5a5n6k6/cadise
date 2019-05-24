#pragma once

#include "core/intersector/intersector.h"

namespace cadise {

class Accelerator : public Intersector {
public:
    virtual bool isIntersecting(Ray &ray, Intersection &intersection) override = 0;
    virtual bool isOccluded(Ray &ray) override = 0;
    virtual RGBColor emittance(Vector3F direction) override;

    virtual Vector3F evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) override;
    virtual Vector3F evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) override;
};

} // namespace cadise