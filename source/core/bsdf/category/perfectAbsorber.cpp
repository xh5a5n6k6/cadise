#include "core/bsdf/category/perfectAbsorber.h"

namespace cadise {

PerfectAbsorber::PerfectAbsorber() :
    Bsdf(BsdfType(BxdfType::DIFFUSE_REFLECTION)) {
}

Spectrum PerfectAbsorber::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum PerfectAbsorber::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

real PerfectAbsorber::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise