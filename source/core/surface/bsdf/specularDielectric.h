#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class DielectricFresnel;
template<typename T>
class Texture;

class SpecularDielectric : public Bsdf {
public:
    SpecularDielectric(
        const std::shared_ptr<Texture<Spectrum>>& albedo,
        const std::shared_ptr<DielectricFresnel>& fresnel);

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
    std::shared_ptr<DielectricFresnel> _fresnel;
};

} // namespace cadise