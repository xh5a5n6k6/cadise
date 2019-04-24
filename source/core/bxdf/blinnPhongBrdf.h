#pragma once

#include "core/bxdf/bxdf.h"

namespace cadise {

class BlinnPhongBRDF : public BxDF {
public:
    BlinnPhongBRDF(float exponent = 32.0f);

    Vector3 evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3 evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    float _exponent;
};

} // namespace cadise