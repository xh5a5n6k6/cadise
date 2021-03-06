#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class DielectricFresnel;
template<typename T>
class TTexture;

class SpecularTransmission : public Bsdf {
public:
    SpecularTransmission(
        const std::shared_ptr<TTexture<Spectrum>>& albedo, 
        const std::shared_ptr<DielectricFresnel>&  fresnel);

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
    std::shared_ptr<TTexture<Spectrum>> _albedo;
    std::shared_ptr<DielectricFresnel>  _fresnel;
};

} // namespace cadise