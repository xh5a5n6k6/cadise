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

    Spectrum evaluateSample(
        const TransportInfo& transportInfo,
        SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(
        const TransportInfo&       transportInfo,
        const SurfaceIntersection& surfaceIntersection) const override;

private:
    std::shared_ptr<Microfacet>       _microfacet;
    std::shared_ptr<ConductorFresnel> _fresnel;
    std::shared_ptr<Texture<real>>    _roughness;
};

} // namespace cadise