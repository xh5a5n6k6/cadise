#include "Core/Film/Filter/GaussianFilter.h"

#include <algorithm>
#include <cmath>

namespace cadise
{

GaussianFilter::GaussianFilter(
    const real filterWidth,
    const real filterHeight,
    const real sigmaX,
    const real sigmaY,
    const real amplitude) :

    Filter(filterWidth, filterHeight),
    _amplitude(amplitude)
{
    const Vector2R halfSize = filterHalfSize();

    _expX = -1.0_r / (2.0_r * sigmaX * sigmaX);
    _expY = -1.0_r / (2.0_r * sigmaY * sigmaY);

    _edgeValue = _amplitude * std::exp(
        _expX * halfSize.x() * halfSize.x() +
        _expY * halfSize.y() * halfSize.y());
}

real GaussianFilter::evaluate(const real locationX, const real locationY) const
{
    const real value = _amplitude * std::exp(
        _expX * locationX * locationX +
        _expY * locationY * locationY);

    return std::max(0.0_r, value - _edgeValue);
}

} // namespace cadise