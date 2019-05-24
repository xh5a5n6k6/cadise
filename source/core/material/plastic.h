#pragma once

#include "core/material/material.h"

namespace cadise {

class Plastic : public Material {
public:
    Plastic(Vector3F albedo, float exponent, float diffuseRatio);

    Vector3F evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3F evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    float _diffuseRatio;
};

} // namespace cadise