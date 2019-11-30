#pragma once

#include "core/bsdf/bsdf.h"

namespace cadise {

class AbsorberBsdf : public Bsdf {
public:
    AbsorberBsdf();

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const override;
};

} // namespace cadise