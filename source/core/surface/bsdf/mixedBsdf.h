#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

template<typename T>
class TTexture;

class MixedBsdf : public Bsdf {
public:
    MixedBsdf(
        const std::shared_ptr<Bsdf>& bsdfA, 
        const std::shared_ptr<Bsdf>& bsdfB,
        const real                   ratio);
    MixedBsdf(
        const std::shared_ptr<Bsdf>&               bsdfA, 
        const std::shared_ptr<Bsdf>&               bsdfB,
        const std::shared_ptr<TTexture<Spectrum>>& ratio);

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

    ELobe lobe(const BsdfComponents component) const override;

private:
    std::shared_ptr<Bsdf> _bsdfA;
    std::shared_ptr<Bsdf> _bsdfB;

    // result = _ratio*_bsdfA + (1 - _ratio)*_bsdfB
    std::shared_ptr<TTexture<Spectrum>> _ratio;
};

} // namespace cadise