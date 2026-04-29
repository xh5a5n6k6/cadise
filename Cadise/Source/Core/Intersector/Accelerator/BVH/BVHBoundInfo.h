#pragma once

#include "Math/TAABB3.h"

namespace cadise
{

class BVHBoundInfo
{
public:
    BVHBoundInfo();
    BVHBoundInfo(const AABB3R& bound, const std::size_t index);

    const AABB3R& bound() const;
    const Vector3R& centroid() const;
    std::size_t index() const;

private:
    AABB3R      _bound;
    Vector3R    _centroid;

    // point to original intersector array index
    std::size_t _index;
};

} // namespace cadise