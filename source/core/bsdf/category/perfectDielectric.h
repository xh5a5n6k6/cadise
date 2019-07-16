#pragma once

#include "core/bsdf/bsdf.h"

#include "core/bsdf/fresnel/dielectricFresnel.h"

namespace cadise {

class PerfectDielectric : public BSDF {
public:
    PerfectDielectric();
    PerfectDielectric(const Spectrum& albedo, const real iorOuter, const real iorInner);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    Spectrum _albedo;
    DielectricFresnel _fresnel;
};

} // namespace cadise