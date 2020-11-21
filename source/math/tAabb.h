#pragma once

#include "math/tVector.h"

namespace cadise {

// AABB is for axis-aligned bounding box
template<typename T, std::size_t N>
class TAABB {
public:
    TAABB();
    explicit TAABB(const TVector<T, N>& vertex);
    TAABB(const TVector<T, N>& minVertex, const TVector<T, N>& maxVertex);

    bool isEmpty() const;
    bool isInside(const TVector<T, N>& position) const;

    bool isIntersectingAABB(
        const TVector<T, N>& rayOrigin, 
        const TVector<T, N>& rayInverseDirection, 
        const T              rayMinT, 
        const T              rayMaxT) const;

    bool isIntersectingAABB(
        const TVector<T, N>& rayOrigin,
        const TVector<T, N>& rayInverseDirection,
        const T              rayMinT,
        const T              rayMaxT,
        T* const             out_boundMinT,
        T* const             out_boundMaxT) const;

    void reset();
    T surfaceArea() const;
    T halfSurfaceArea() const;
    TVector<T, N> extent() const;
    TVector<T, N> centroid() const;
    std::size_t maxAxis() const;

    TAABB& unionWith(const TVector<T, N>& vertex);
    TAABB& unionWith(const TAABB& aabb);
    TAABB& expand(const T scalar);

    const TVector<T, N>& minVertex() const;
    const TVector<T, N>& maxVertex() const;

private:
    TVector<T, N> _minVertex;
    TVector<T, N> _maxVertex;
};

} // namespace cadise

#include "math/tAabb.ipp"