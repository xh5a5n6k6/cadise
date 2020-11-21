#pragma once

#include "math/tAabb.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <limits>
#include <type_traits>

namespace cadise {

template<typename T, std::size_t N>
inline cadise::TAABB<T, N>::TAABB() :
    TAABB(TVector<T, N>(std::numeric_limits<T>::max()), 
          TVector<T, N>(std::numeric_limits<T>::min())) {
}

template<typename T, std::size_t N>
inline cadise::TAABB<T, N>::TAABB(const TVector<T, N>& vertex) :
    TAABB(vertex, vertex) {
}

template<typename T, std::size_t N>
inline cadise::TAABB<T, N>::TAABB(const TVector<T, N>& minVertex, const TVector<T, N>& maxVertex) :
    _minVertex(minVertex),
    _maxVertex(maxVertex) { 
}

template<typename T, std::size_t N>
inline bool TAABB<T, N>::isEmpty() const {
    return _minVertex.isEqualTo(TVector<T, N>(std::numeric_limits<T>::max())) ||
           _maxVertex.isEqualTo(TVector<T, N>(std::numeric_limits<T>::min()));
}

template<typename T, std::size_t N>
inline bool TAABB<T, N>::isInside(const TVector<T, N>& position) const {
    for (std::size_t i = 0; i < N; ++i) {
        if (position[i] < _minVertex[i] || position[i] > _maxVertex[i]) {
            return false;
        }
    }

    return true;
}

template<typename T, std::size_t N>
inline bool TAABB<T, N>::isIntersectingAABB(
    const TVector<T, N>& rayOrigin, 
    const TVector<T, N>& rayInverseDirection, 
    const T              rayMinT, 
    const T              rayMaxT) const {

    static_assert(N == 3, "Not support isIntersecting with this kind of AABB");
    
    T localMinT;
    T localMaxT;

    return this->isIntersectingAABB(
        rayOrigin, 
        rayInverseDirection, 
        rayMinT, 
        rayMaxT,
        &localMinT,
        &localMaxT);
}

template<typename T, std::size_t N>
inline bool TAABB<T, N>::isIntersectingAABB(
    const TVector<T, N>& rayOrigin,
    const TVector<T, N>& rayInverseDirection,
    const T              rayMinT,
    const T              rayMaxT,
    T* const             out_boundMinT,
    T* const             out_boundMaxT) const {

    static_assert(N == 3, "Not support isIntersecting with this kind of AABB");

    CADISE_ASSERT(out_boundMinT);
    CADISE_ASSERT(out_boundMaxT);

    T minT = rayMinT;
    T maxT = rayMaxT;

    const TVector<T, N> nearT = (_minVertex - rayOrigin) * rayInverseDirection;
    const TVector<T, N> farT  = (_maxVertex - rayOrigin) * rayInverseDirection;

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
inline void TAABB<T, N>::reset() {
    _minVertex = TVector<T, N>(std::numeric_limits<T>::max());
    _maxVertex = TVector<T, N>(std::numeric_limits<T>::min());
}

template<typename T, std::size_t N>
inline T TAABB<T, N>::surfaceArea() const {
    static_assert(N == 3, "Not support surfaceArea with this kind of AABB");

    return static_cast<T>(2) * this->halfSurfaceArea();
}

template<typename T, std::size_t N>
inline T TAABB<T, N>::halfSurfaceArea() const {
    static_assert(N == 3, "Not support surfaceArea with this kind of AABB");

    const TVector<T, N> extent = this->extent();

    return extent.x() * extent.y() +
           extent.x() * extent.z() +
           extent.y() * extent.z();
}

template<typename T, std::size_t N>
inline TVector<T, N> TAABB<T, N>::extent() const {
    return _maxVertex - _minVertex;
}

template<typename T, std::size_t N>
inline TVector<T, N> TAABB<T, N>::centroid() const {
    if constexpr (std::is_integral_v<T>) {
        return (_minVertex + _maxVertex) / static_cast<T>(2);
    }
    else { 
        return (_minVertex + _maxVertex) * static_cast<T>(0.5_r); 
    }
}

template<typename T, std::size_t N>
inline std::size_t TAABB<T, N>::maxAxis() const {
    return this->extent().maxDimension();
}

template<typename T, std::size_t N>
inline TAABB<T, N>& TAABB<T, N>::unionWith(const TVector<T, N>& vertex) {
    _minVertex = TVector<T, N>::min(_minVertex, vertex);
    _maxVertex = TVector<T, N>::max(_maxVertex, vertex);
    
    return *this;
}

template<typename T, std::size_t N>
inline TAABB<T, N>& TAABB<T, N>::unionWith(const TAABB<T, N>& aabb) {
    _minVertex = TVector<T, N>::min(_minVertex, aabb._minVertex);
    _maxVertex = TVector<T, N>::max(_maxVertex, aabb._maxVertex);

    return *this;
}

template<typename T, std::size_t N>
inline TAABB<T, N>& TAABB<T, N>::expand(const T scalar) {
    _minVertex -= scalar;
    _maxVertex += scalar;

    return *this;
}

template<typename T, std::size_t N>
inline const TVector<T, N>& TAABB<T, N>::minVertex() const {
    return _minVertex;
}

template<typename T, std::size_t N>
inline const TVector<T, N>& TAABB<T, N>::maxVertex() const {
    return _maxVertex;
}

} // namespace cadise