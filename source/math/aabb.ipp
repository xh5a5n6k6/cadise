#pragma once

#include "math/aabb.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <limits>
#include <type_traits>

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
    const Vector<T, N>& rayOrigin, 
    const Vector<T, N>& rayInverseDirection, 
    const T             rayMinT, 
    const T             rayMaxT) const {

    static_assert(N == 3, "Not support isIntersecting with this kind of AABB");
    
    T localMinT;
    T localMaxT;

    return isIntersectingAABB(rayOrigin, 
                              rayInverseDirection, 
                              rayMinT, 
                              rayMaxT,
                              &localMinT,
                              &localMaxT);
}

template<typename T, std::size_t N>
inline bool AABB<T, N>::isIntersectingAABB(
    const Vector<T, N>& rayOrigin,
    const Vector<T, N>& rayInverseDirection,
    const T             rayMinT,
    const T             rayMaxT,
    T* const            out_boundMinT,
    T* const            out_boundMaxT) const {

    static_assert(N == 3, "Not support isIntersecting with this kind of AABB");

    CADISE_ASSERT(out_boundMinT);
    CADISE_ASSERT(out_boundMaxT);

    T minT = rayMinT;
    T maxT = rayMaxT;

    const Vector<T, N> nearT = (_minVertex - rayOrigin) * rayInverseDirection;
    const Vector<T, N> farT  = (_maxVertex - rayOrigin) * rayInverseDirection;

    // calculate x-slab interval
    if (rayInverseDirection.x() > static_cast<T>(0)) {
        minT = math::max(minT, nearT.x());
        maxT = math::min(maxT, farT.x());
    }
    else {
        minT = math::max(minT, farT.x());
        maxT = math::min(maxT, nearT.x());
    }

    // calculate y-slab interval
    if (rayInverseDirection.y() > static_cast<T>(0)) {
        minT = math::max(minT, nearT.y());
        maxT = math::min(maxT, farT.y());
    }
    else {
        minT = math::max(minT, farT.y());
        maxT = math::min(maxT, nearT.y());
    }

    // calculate z-slab interval
    if (rayInverseDirection.z() > static_cast<T>(0)) {
        minT = math::max(minT, nearT.z());
        maxT = math::min(maxT, farT.z());
    }
    else {
        minT = math::max(minT, farT.z());
        maxT = math::min(maxT, nearT.z());
    }

    // check if intersection exists
    if (minT > maxT) {
        return false;
    }

    *out_boundMinT = minT;
    *out_boundMaxT = maxT;

    return true;
}

template<typename T, std::size_t N>
inline T AABB<T, N>::surfaceArea() const {
    static_assert(N == 3, "Not support surfaceArea with this kind of AABB");

    return static_cast<T>(2) * halfSurfaceArea();
}

template<typename T, std::size_t N>
inline T AABB<T, N>::halfSurfaceArea() const {
    static_assert(N == 3, "Not support surfaceArea with this kind of AABB");

    const Vector<T, N> extent = extent();

    return extent.x() * extent.y() +
           extent.x() * extent.z() +
           extent.y() * extent.z();
}

template<typename T, std::size_t N>
inline Vector<T, N> AABB<T, N>::extent() const {
    return _maxVertex - _minVertex;
}

template<typename T, std::size_t N>
inline Vector<T, N> AABB<T, N>::centroid() const {
    if constexpr (std::is_integral_v<T>) {
        return (_minVertex + _maxVertex) / static_cast<T>(2);
    }
    else { 
        return (_minVertex + _maxVertex) * static_cast<T>(0.5_r); 
    }
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
inline const Vector<T, N>& AABB<T, N>::minVertex() const {
    return _minVertex;
}

template<typename T, std::size_t N>
inline const Vector<T, N>& AABB<T, N>::maxVertex() const {
    return _maxVertex;
}

} // namespace cadise