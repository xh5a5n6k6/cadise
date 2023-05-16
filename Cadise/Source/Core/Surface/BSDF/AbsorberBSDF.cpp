#include "Core/Surface/BSDF/AbsorberBSDF.h"

#include "Foundation/Assertion.h"

namespace cadise
{

AbsorberBSDF::AbsorberBSDF() :
    BSDF(BSDFLobes({ ELobe::Absorb }))
{}

Spectrum AbsorberBSDF::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const
{
    return Spectrum(0.0_r);
}

void AbsorberBSDF::evaluateSample(
    const TransportInfo&       info,
    const SurfaceIntersection& si,
    BSDFSample* const          out_sample) const
{
    CS_ASSERT(out_sample);

    return;
}

real AbsorberBSDF::evaluatePdfW(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const
{
    return 0.0_r;
}

ELobe AbsorberBSDF::lobe(const BSDFComponents component) const
{
    CS_ASSERT_EQ(component, 0);

    return ELobe::Absorb;
}

} // namespace cadise