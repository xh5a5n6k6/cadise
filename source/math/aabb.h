#pragma once

#include "math/vector.h"

namespace cadise {

template<typename T, uint64 Size>
class AABB;

using AABB2I = AABB<int, 2>;
using AABB3F = AABB<float, 3>;

// AABB is for axis-aligned bounding box
template<typename T, uint64 Size>
class AABB {
public:
    AABB();
    AABB(Vector<T, Size> vertex);
    AABB(Vector<T, Size> minVertex, Vector<T, Size> maxVertex);

    bool isOccluded();
    Vector<T, Size> center();

    AABB unionWith(Vector<T, Size> vertex);
    AABB unionWith(AABB aabb);

private:
    Vector<T, Size> _minVertex;
    Vector<T, Size> _maxVertex;
};

} // namespace cadise

#include "math/aabb.ipp"