#pragma once

#include "core/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class BlinnPhong : public Bsdf {
public:
    BlinnPhong(const real exponent);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const override;

private:
    real _exponent;
};

} // namespace cadise