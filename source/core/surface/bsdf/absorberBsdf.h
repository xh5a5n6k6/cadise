#pragma once

#include "core/surface/bsdf/bsdf.h"

namespace cadise {

class AbsorberBsdf : public Bsdf {
public:
    AbsorberBsdf();

    Spectrum evaluate(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const override;

    void evaluateSample(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection,
        BsdfSample* const          out_sample) const override;

    real evaluatePdfW(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const override;
};

} // namespace cadise