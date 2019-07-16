#pragma once

#include "core/bsdf/bsdf.h"

namespace cadise {

class LambertianDiffuse : public BSDF {
public:
    LambertianDiffuse(const Spectrum& albedo);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    Spectrum _albedo;
};

} // namespace cadise