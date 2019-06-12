#pragma once

#include "core/material/material.h"

namespace cadise {

class Plastic : public Material {
public:
    Plastic(const Vector3R albedo, const real exponent, const real diffuseRatio);

    Vector3R evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const override;
    Vector3R evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const override;

private:
    real _diffuseRatio;
};

} // namespace cadise