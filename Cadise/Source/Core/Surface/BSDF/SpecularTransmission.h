#pragma once

#include "Core/Surface/BSDF/BSDF.h"

#include <memory>

// forward declaration
namespace cadise
{
    class DielectricFresnel;

    template<typename T>
    class TTexture;
}

namespace cadise
{

class SpecularTransmission : public BSDF
{
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
        BSDFSample* const          out_sample) const override;

    real evaluatePdfW(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const override;

    ELobe lobe(const BSDFComponents component) const override;

private:
    std::shared_ptr<TTexture<Spectrum>> _albedo;
    std::shared_ptr<DielectricFresnel>  _fresnel;
};

} // namespace cadise