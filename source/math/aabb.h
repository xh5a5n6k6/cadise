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
    AABB(Vector<T, Size> vertex);
    AABB(Vector<T, Size> minVertex, Vector<T, Size> maxVertex);

    bool isIntersecting(const Vector<T, Size> origin, const Vector<T, Size> invDirection, real tmin, real tmax);
    Vector<T, Size> center();

    AABB unionWith(Vector<T, Size> vertex);
    AABB unionWith(AABB aabb);

private:
    Vector<T, Size> _minVertex;
    Vector<T, Size> _maxVertex;
};

} // namespace cadise

#include "math/aabb.ipp"