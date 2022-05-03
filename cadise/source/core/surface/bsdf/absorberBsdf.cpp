#include "core/surface/bsdf/absorberBsdf.h"

#include "fundamental/assertion.h"

namespace cadise 
{

AbsorberBsdf::AbsorberBsdf() :
    Bsdf(BsdfLobes({ ELobe::Absorb }))
{}

Spectrum AbsorberBsdf::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const
{
    return Spectrum(0.0_r);
}

void AbsorberBsdf::evaluateSample(
    const TransportInfo&       info,
    const SurfaceIntersection& si,
    BsdfSample* const          out_sample) const
{
    CS_ASSERT(out_sample);

    return;
}

real AbsorberBsdf::evaluatePdfW(
    const TransportInfo&       info, 
    const SurfaceIntersection& si) const
{
    return 0.0_r;
}

ELobe AbsorberBsdf::lobe(const BsdfComponents component) const 
{
    CS_ASSERT_EQ(component, 0);

    return ELobe::Absorb;
}

} // namespace cadise