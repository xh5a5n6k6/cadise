#include "core/bsdf/category/absorberBsdf.h"

namespace cadise {

AbsorberBsdf::AbsorberBsdf() :
    Bsdf(BsdfType(BxdfType::ABSORB)) {
}

Spectrum AbsorberBsdf::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum AbsorberBsdf::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

real AbsorberBsdf::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise