#pragma once

#include "math/vector.h"

namespace cadise {

// AABB is for axis-aligned bounding box
template<typename T, std::size_t N>
class AABB {
public:
    AABB();
    explicit AABB(const Vector<T, N>& vertex);
    AABB(const Vector<T, N>& minVertex, const Vector<T, N>& maxVertex);

    bool isEmpty() const;

    bool isIntersectingAABB(
        const Vector<T, N>& rayOrigin, 
        const Vector<T, N>& rayInverseDirection, 
        const T             rayMinT, 
        const T             rayMaxT) const;

    bool isIntersectingAABB(
        const Vector<T, N>& rayOrigin,
        const Vector<T, N>& rayInverseDirection,
        const T             rayMinT,
        const T             rayMaxT,
        T* const            out_boundMinT,
        T* const            out_boundMaxT) const;

    T surfaceArea() const;
    T halfSurfaceArea() const;
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