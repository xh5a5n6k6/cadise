#include "core/surface/bsdf/absorberBsdf.h"

#include "fundamental/assertion.h"

namespace cadise {

AbsorberBsdf::AbsorberBsdf() :
    Bsdf(BsdfType(BxdfType::ABSORB)) {
}

Spectrum AbsorberBsdf::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {

    return Spectrum(0.0_r);
}

void AbsorberBsdf::evaluateSample(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection,
    BsdfSample* const          out_sample) const {

    CADISE_ASSERT(out_sample);

    return;
}

real AbsorberBsdf::evaluatePdfW(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection) const {

    return 0.0_r;
}

} // namespace cadise