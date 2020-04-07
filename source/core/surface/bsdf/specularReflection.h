#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class Fresnel;
template<typename T>
class Texture;

class SpecularReflection : public Bsdf {
public:
    SpecularReflection(
        const std::shared_ptr<Texture<Spectrum>>& albedo,
        const std::shared_ptr<Fresnel>&           fresnel);

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
    std::shared_ptr<Fresnel>           _fresnel;
};

} // namespace cadise