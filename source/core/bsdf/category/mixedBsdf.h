#pragma once

#include "core/bsdf/bsdf.h"

#include "core/spectrum/spectrum.h"

#include <memory>

namespace cadise {

template<typename T>
class Texture;

class MixedBsdf : public Bsdf {
public:
    MixedBsdf(const std::shared_ptr<Bsdf>& bsdfA, 
              const std::shared_ptr<Bsdf>& bsdfB,
              const real ratio);
    MixedBsdf(const std::shared_ptr<Bsdf>& bsdfA, 
              const std::shared_ptr<Bsdf>& bsdfB,
              const std::shared_ptr<Texture<Spectrum>>& ratio);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const override;

private:
    std::shared_ptr<Bsdf> _bsdfA;
    std::shared_ptr<Bsdf> _bsdfB;

    // result = _ratio*_bsdfA + (1 - _ratio)*_bsdfB
    std::shared_ptr<Texture<Spectrum>> _ratio;
};

} // namespace cadise