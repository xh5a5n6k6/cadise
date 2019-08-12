#pragma once

#include "core/bsdf/bsdf.h"

#include "core/bsdf/fresnel/dielectricFresnel.h"

#include <memory>

namespace cadise {

template<typename T>
class Texture;

class SpecularTransmission : public BSDF {
public:
    SpecularTransmission(const std::shared_ptr<Texture<Spectrum>>& albedo, 
                         const real iorOuter, const real iorInner);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    std::shared_ptr<Texture<Spectrum>> _albedo;
    DielectricFresnel _fresnel;
};

} // namespace cadise