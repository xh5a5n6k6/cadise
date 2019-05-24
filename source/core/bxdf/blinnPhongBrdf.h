#pragma once

#include "core/bxdf/bxdf.h"

namespace cadise {

class BlinnPhongBRDF : public BxDF {
public:
    BlinnPhongBRDF(float exponent = 32.0f);

    Vector3F evaluate(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3F evaluateSample(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    float _exponent;
};

} // namespace cadise