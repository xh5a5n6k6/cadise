#pragma once

#include "core/bsdf/bsdf.h"

namespace cadise {

class BlinnPhong : public BSDF {
public:
    BlinnPhong(const real exponent = 32.0_r);

    Vector3R evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Vector3R evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    real _exponent;
};

} // namespace cadise