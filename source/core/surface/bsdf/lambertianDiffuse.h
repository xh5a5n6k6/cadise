#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

template<typename T>
class Texture;

class LambertianDiffuse : public Bsdf {
public:
    // Hack
    LambertianDiffuse();
    explicit LambertianDiffuse(const std::shared_ptr<Texture<Spectrum>>& albedo);

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

private:
    std::shared_ptr<Texture<Spectrum>> _albedo;
};

} // namespace cadise