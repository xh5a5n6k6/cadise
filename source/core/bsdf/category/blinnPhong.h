#pragma once

#include "core/bsdf/bsdf.h"

namespace cadise {

class BlinnPhong : public Bsdf {
public:
    BlinnPhong(const real exponent = 32.0_r);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    real _exponent;
};

} // namespace cadise