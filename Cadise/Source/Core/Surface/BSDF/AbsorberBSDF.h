#pragma once

#include "Core/surface/bsdf/BSDF.h"

namespace cadise 
{

class AbsorberBSDF : public BSDF 
{
public:
    AbsorberBSDF();

    Spectrum evaluate(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const override;

    void evaluateSample(
        const TransportInfo&       info,
        const SurfaceIntersection& si,
        BSDFSample* const          out_sample) const override;

    real evaluatePdfW(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const override;

    ELobe lobe(const BSDFComponents component) const override;
};

} // namespace cadise