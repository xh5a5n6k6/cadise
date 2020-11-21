#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class Fresnel;
template<typename T>
class TTexture;

class SpecularReflection : public Bsdf {
public:
    SpecularReflection(
        const std::shared_ptr<TTexture<Spectrum>>& albedo,
        const std::shared_ptr<Fresnel>&            fresnel);

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
    std::shared_ptr<TTexture<Spectrum>> _albedo;
    std::shared_ptr<Fresnel>            _fresnel;
};

} // namespace cadise