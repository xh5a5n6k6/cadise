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

    Spectrum evaluateSample(
        const TransportInfo& transportInfo,
        SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const override;

private:
    std::shared_ptr<Texture<Spectrum>> _albedo;
};

} // namespace cadise