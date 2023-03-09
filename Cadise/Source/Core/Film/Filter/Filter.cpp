#include "core/film/filter/filter.h"

namespace cadise 
{

Filter::Filter(const real filterWidth, const real filterHeight) :
    _filterSize(filterWidth, filterHeight)
{}

Filter::~Filter() = default;

const Vector2R& Filter::filterSize() const
{
    return _filterSize;
}

Vector2R Filter::filterHalfSize() const
{
    return _filterSize.mul(0.5_r);
}

} // namespace cadise