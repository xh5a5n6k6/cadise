#pragma once

#include "math/vector.h"

namespace cadise {

template<typename T, uint32 Size>
class AABB;

using AABB2I = AABB<int32, 2>;
using AABB3R = AABB<real, 3>;

// AABB is for axis-aligned bounding box
template<typename T, uint32 Size>
class AABB {
public:
    AABB();
    AABB(const Vector<T, Size>& vertex);
    AABB(const Vector<T, Size>& minVertex, const Vector<T, Size>& maxVertex);

    bool isIntersectingAABB(const Vector<T, Size>& origin, const Vector<T, Size>& invDirection, real tmin, real tmax) const;
    Vector<T, Size> centroid() const;
    uint32 maxAxis() const;

    AABB& unionWith(const Vector<T, Size>& vertex);
    AABB& unionWith(const AABB& aabb);
    AABB& expand(const T scalar);

private:
    Vector<T, Size> _minVertex;
    Vector<T, Size> _maxVertex;
};

} // namespace cadise

#include "math/aabb.ipp"