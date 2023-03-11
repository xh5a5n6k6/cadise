#include "Core/Film/Filter/MitchellFilter.h"

#include <cmath>

namespace cadise
{

MitchellFilter::MitchellFilter(
    const real filterWidth, 
    const real filterHeight,
    const real b, 
    const real c) :

    Filter(filterWidth, filterHeight),
    _b(b),
    _c(c)
{}

real MitchellFilter::evaluate(const real locationX, const real locationY) const 
{
    return _mitchell1D(locationX) * _mitchell1D(locationY);
}

real MitchellFilter::_mitchell1D(const real d) const 
{
    const real absD = std::abs(d);
    const real denominator = 1.0_r / 6.0_r;

    if (absD < 1.0_r) 
    {
        return ((12.0_r - 9.0_r * _b - 6.0_r * _c) * absD * absD * absD
                + (-18.0_r + 12.0_r * _b + 6.0_r * _c) * absD * absD
                + (6.0_r - 2.0_r * _b)
                ) * denominator;
    }
    else if (absD < 2.0_r)
    {
        return ((-_b - 6.0_r * _c) * absD * absD * absD
                + (6.0_r * _b + 30.0_r * _c) * absD * absD
                + (-12.0_r * _b - 48.0_r * _c) * absD
                + (8.0_r * _b + 24.0_r * _c)
                ) * denominator;
    }
    else 
    {
        return 0.0_r;
    }
}

} // namespace cadise