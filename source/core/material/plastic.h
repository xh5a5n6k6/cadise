#pragma once

#include "core/material/material.h"

namespace cadise {

class Plastic : public Material {
public:
    Plastic(Vector3R albedo, real exponent, real diffuseRatio);

    Vector3R evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3R evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    real _diffuseRatio;
};

} // namespace cadise