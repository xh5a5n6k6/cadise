#pragma once

#include "core/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class DielectricFresnel;
template<typename T>
class Texture;

class SpecularDielectric : public Bsdf {
public:
    SpecularDielectric(const std::shared_ptr<Texture<Spectrum>>& albedo,
                       const std::shared_ptr<DielectricFresnel>& fresnel);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const override;

private:
    std::shared_ptr<Texture<Spectrum>> _albedo;
    std::shared_ptr<DielectricFresnel> _fresnel;
};

} // namespace cadise