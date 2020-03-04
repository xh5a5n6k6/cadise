#include "core/surface/bsdf/absorberBsdf.h"

namespace cadise {

AbsorberBsdf::AbsorberBsdf() :
    Bsdf(BsdfType(BxdfType::ABSORB)) {
}

Spectrum AbsorberBsdf::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {

    return Spectrum(0.0_r);
}

Spectrum AbsorberBsdf::evaluateSample(
    const TransportInfo& transportInfo,
    SurfaceIntersection& surfaceIntersection) const {

    return Spectrum(0.0_r);
}

real AbsorberBsdf::evaluatePdfW(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection) const {

    return 0.0_r;
}

} // namespace cadise