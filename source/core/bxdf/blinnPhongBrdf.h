#pragma once

#include "core/bxdf/bxdf.h"

namespace cadise {

class BlinnPhongBRDF : public BxDF {
public:
    BlinnPhongBRDF(real exponent = 32.0_r);

    Vector3R evaluate(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) override;
    Vector3R evaluateSample(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) override;

private:
    real _exponent;
};

} // namespace cadise