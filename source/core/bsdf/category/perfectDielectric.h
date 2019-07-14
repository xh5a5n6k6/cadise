#pragma once

#include "core/bsdf/bsdf.h"

#include "core/bsdf/fresnel/dielectricFresnel.h"

namespace cadise {

class PerfectDielectric : public BSDF {
public:
    PerfectDielectric();
    PerfectDielectric(const Vector3R& albedo, const real iorOuter, const real iorInner);

    Vector3R evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Vector3R evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

private:
    Vector3R _albedo;
    DielectricFresnel _fresnel;
};

} // namespace cadise