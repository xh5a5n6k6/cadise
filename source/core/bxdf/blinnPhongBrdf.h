#pragma once

#include "core/bxdf/bxdf.h"

namespace cadise {

class BlinnPhongBRDF : public BxDF {
public:
    BlinnPhongBRDF(const real exponent = 32.0_r);

    Vector3R evaluate(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const override;
    Vector3R evaluateSample(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const override;

private:
    real _exponent;
};

} // namespace cadise