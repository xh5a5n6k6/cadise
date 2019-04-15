#pragma once

#include "core/material/material.h"

namespace cadise {

class Plastic : public Material {
public:
    Plastic(Vector3 albedo, float exponent, float diffuseRatio);

    Vector3 evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3 evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    float _diffuseRatio;
};

} // namespace cadise