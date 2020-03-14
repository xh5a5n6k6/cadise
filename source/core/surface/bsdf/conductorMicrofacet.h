#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class ConductorFresnel;
class Microfacet;
template<typename T>
class Texture;

class ConductorMicrofacet : public Bsdf {
public:
    ConductorMicrofacet(
        const std::shared_ptr<Microfacet>&       microfacet,
        const std::shared_ptr<ConductorFresnel>& fresnel,
        const std::shared_ptr<Texture<real>>&    roughness);

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
    std::shared_ptr<Microfacet>       _microfacet;
    std::shared_ptr<ConductorFresnel> _fresnel;
    std::shared_ptr<Texture<real>>    _roughness;
};

} // namespace cadise