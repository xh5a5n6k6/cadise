#pragma once

#include "core/surface/bsdf/bsdf.h"

#include <memory>

namespace cadise {

class ConductorFresnel;
class Microfacet;

class ConductorMicrofacet : public Bsdf {
public:
    ConductorMicrofacet(
        const std::shared_ptr<Microfacet>&       microfacet,
        const std::shared_ptr<ConductorFresnel>& fresnel);

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
    std::shared_ptr<Microfacet>       _microfacet;
    std::shared_ptr<ConductorFresnel> _fresnel;
};

} // namespace cadise