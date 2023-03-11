#pragma once

#include "Core/Surface/BSDF/BSDF.h"

#include <memory>

// forward declaration
namespace cadise
{
    template<typename T>
    class TTexture;
}

namespace cadise
{

class MixedBSDF : public BSDF
{
public:
    MixedBSDF(
        const std::shared_ptr<BSDF>& bsdfA, 
        const std::shared_ptr<BSDF>& bsdfB,
        const real                   ratio);
    MixedBSDF(
        const std::shared_ptr<BSDF>&               bsdfA, 
        const std::shared_ptr<BSDF>&               bsdfB,
        const std::shared_ptr<TTexture<Spectrum>>& ratio);

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

private:
    std::shared_ptr<BSDF> _bsdfA;
    std::shared_ptr<BSDF> _bsdfB;

    // result = _ratio*_bsdfA + (1 - _ratio)*_bsdfB
    std::shared_ptr<TTexture<Spectrum>> _ratio;
};

} // namespace cadise