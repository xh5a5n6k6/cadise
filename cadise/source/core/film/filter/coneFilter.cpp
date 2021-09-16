#include "core/film/filter/coneFilter.h"

#include <algorithm>
#include <cmath>

namespace cadise
{

ConeFilter::ConeFilter(const real filterWidth, const real filterHeight) :
    Filter(filterWidth, filterHeight)
{}

real ConeFilter::evaluate(const real locationX, const real locationY) const
{
    return std::max(filterHalfSize().x() - std::abs(locationX), 0.0_r) *
           std::max(filterHalfSize().y() - std::abs(locationY), 0.0_r);
}

} // namespace cadise