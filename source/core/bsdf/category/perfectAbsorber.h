#pragma once

#include "core/bsdf/bsdf.h"

namespace cadise {

class PerfectAbsorber : public Bsdf {
public:
    PerfectAbsorber();

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const override;
};

} // namespace cadise