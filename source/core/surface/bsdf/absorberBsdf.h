#pragma once

#include "core/surface/bsdf/bsdf.h"

namespace cadise {

class AbsorberBsdf : public Bsdf {
public:
    AbsorberBsdf();

    Spectrum evaluate(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const override;

    Spectrum evaluateSample(
        const TransportInfo& transportInfo,
        SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const override;
};

} // namespace cadise