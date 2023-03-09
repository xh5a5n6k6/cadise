#include "core/film/filter/boxFilter.h"

namespace cadise
{

BoxFilter::BoxFilter(const real filterWidth, const real filterHeight) :
    Filter(filterWidth, filterHeight)
{}

real BoxFilter::evaluate(const real locationX, const real locationY) const
{
    return 1.0_r;
}

} // namespace cadise