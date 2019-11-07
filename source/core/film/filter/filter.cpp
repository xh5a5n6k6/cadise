#include "core/film/filter/filter.h"

namespace cadise {

Filter::Filter(const real filterWidth, const real filterHeight) :
    _filterSize(filterWidth, filterHeight) {
}

const Vector2R& Filter::filterSize() const {
    return _filterSize;
}

Vector2R Filter::filterHalfSize() const {
    return _filterSize / 2.0_r;
}

} // namespace cadise