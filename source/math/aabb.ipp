#pragma once

#include "math/aabb.h"

#include <limits>

namespace cadise {

template<typename T, std::size_t N>
inline cadise::AABB<T, N>::AABB() :
    AABB(Vector<T, N>(std::numeric_limits<T>::max()), 
         Vector<T, N>(std::numeric_limits<T>::min())) {
}

template<typename T, std::size_t N>
inline cadise::AABB<T, N>::AABB(const Vector<T, N>& vertex) :
    AABB(vertex, vertex) {
}

template<typename T, std::size_t N>
inline cadise::AABB<T, N>::AABB(const Vector<T, N>& minVertex, const Vector<T, N>& maxVertex) :
    _minVertex(minVertex),
    _maxVertex(maxVertex) { 
}

template<typename T, std::size_t N>
inline bool AABB<T, N>::isIntersectingAABB(
    const Vector<T, N>& origin, 
    const Vector<T, N>& inverseDirection, 
    real minT, 
    real maxT) const {

    static_assert(N == 3, "Not support isIntersecting with this kind of AABB");
    
    const Vector<T, N> nearT = (_minVertex - origin) * inverseDirection;
    const Vector<T, N> farT  = (_maxVertex - origin) * inverseDirection;
    
    // calculate x-slab interval
    if (inverseDirection.x() > 0.0_r) {
        minT = std::max(minT, nearT.x());
        maxT = std::min(maxT,  farT.x());
    }
    else {
        minT = std::max(minT,  farT.x());
        maxT = std::min(maxT, nearT.x());
    }

    // calculate y-slab interval
    if (inverseDirection.y() > 0.0_r) {
        minT = std::max(minT, nearT.y());
        maxT = std::min(maxT,  farT.y());
    }
    else {
        minT = std::max(minT,  farT.y());
        maxT = std::min(maxT, nearT.y());
    }

    // calculate z-slab interval
    if (inverseDirection.z() > 0.0_r) {
        minT = std::max(minT, nearT.z());
        maxT = std::min(maxT,  farT.z());
    }
    else {
        minT = std::max(minT,  farT.z());
        maxT = std::min(maxT, nearT.z());
    }

    // check if intersection exists
    if (minT > maxT) {
        return false;
    }

    return true;
}

template<typename T, std::size_t N>
inline Vector<T, N> AABB<T, N>::extent() const {
    return _maxVertex - _minVertex;
}

template<typename T, std::size_t N>
inline Vector<T, N> AABB<T, N>::centroid() const {
    return (_minVertex + _maxVertex) / static_cast<T>(2);
}

template<typename T, std::size_t N>
inline std::size_t AABB<T, N>::maxAxis() const {
    return extent().maxDimension();
}

template<typename T, std::size_t N>
inline AABB<T, N>& AABB<T, N>::unionWith(const Vector<T, N>& vertex) {
    _minVertex = Vector<T, N>::min(_minVertex, vertex);
    _maxVertex = Vector<T, N>::max(_maxVertex, vertex);
    
    return *this;
}

template<typename T, std::size_t N>
inline AABB<T, N>& AABB<T, N>::unionWith(const AABB<T, N>& aabb) {
    _minVertex = Vector<T, N>::min(_minVertex, aabb._minVertex);
    _maxVertex = Vector<T, N>::max(_maxVertex, aabb._maxVertex);

    return *this;
}

template<typename T, std::size_t N>
inline AABB<T, N>& AABB<T, N>::expand(const T scalar) {
    _minVertex -= scalar;
    _maxVertex += scalar;

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N> AABB<T, N>::minVertex() const {
    return _minVertex;
}

template<typename T, std::size_t N>
inline Vector<T, N> AABB<T, N>::maxVertex() const {
    return _maxVertex;
}

} // namespace cadise