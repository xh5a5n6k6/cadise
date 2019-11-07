#pragma once

#include "math/vector.h"

namespace cadise {

// AABB is for axis-aligned bounding box
template<typename T, std::size_t N>
class AABB {
public:
    AABB();
    AABB(const Vector<T, N>& vertex);
    AABB(const Vector<T, N>& minVertex, const Vector<T, N>& maxVertex);

    bool isIntersectingAABB(
        const Vector<T, N>& origin, 
        const Vector<T, N>& inverseDirection, 
        real minT, 
        real maxT) const;

    Vector<T, N> extent() const;
    Vector<T, N> centroid() const;
    std::size_t maxAxis() const;

    AABB& unionWith(const Vector<T, N>& vertex);
    AABB& unionWith(const AABB& aabb);
    AABB& expand(const T scalar);

    const Vector<T, N>& minVertex() const;
    const Vector<T, N>& maxVertex() const;

private:
    Vector<T, N> _minVertex;
    Vector<T, N> _maxVertex;
};

} // namespace cadise

#include "math/aabb.ipp"