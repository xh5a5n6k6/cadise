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
    LambertianDiffuse(const std::shared_ptr<Texture<Spectrum>>& albedo);

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

private:
    std::shared_ptr<Texture<Spectrum>> _albedo;
};

} // namespace cadise