#pragma once

#include "core/surface/bsdf/bsdf.h"

namespace cadise {

class AbsorberBsdf : public Bsdf {
public:
    AbsorberBsdf();

    Spectrum evaluate(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const override;

    void evaluateSample(
        const TransportInfo&       info,
        const SurfaceIntersection& si,
        BsdfSample* const          out_sample) const override;

    real evaluatePdfW(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const override;
};

} // namespace cadise