#pragma once

#include "math/aabb.h"

#include <limits>
#include <typeinfo>

namespace cadise {

template<typename T, uint32 Size>
inline cadise::AABB<T, Size>::AABB() :
    _minVertex(std::numeric_limits<T>::max()),
    _maxVertex(std::numeric_limits<T>::min()) {
}

template<typename T, uint32 Size>
inline cadise::AABB<T, Size>::AABB(Vector<T, Size> vertex) :
    _minVertex(vertex),
    _maxVertex(vertex) {
}

template<typename T, uint32 Size>
inline cadise::AABB<T, Size>::AABB(Vector<T, Size> minVertex, Vector<T, Size> maxVertex) :
    _minVertex(minVertex),
    _maxVertex(maxVertex) { 
}

template<typename T, uint32 Size>
inline bool AABB<T, Size>::isIntersecting(const Vector<T, Size> origin, const Vector<T, Size> invDirection, real tmin, real tmax) {
    static_assert(Size == 3 && typeid(T) == typeid(real), "Not support isIntersecting with this kind of AABB");
    
    Vector<T, Size> tnear = (_minVertex - origin) * invDirection;
    Vector<T, Size> tfar  = (_maxVertex - origin) * invDirection;
    
    // calculate x-slab interval
    if (invDirection.x() > 0.0_r) {
        tmin = std::max(tmin, tnear.x());
        tmax = std::min(tmax,  tfar.x());
    }
    else {
        tmin = std::max(tmin,  tfar.x());
        tmax = std::min(tmax, tnear.x());
    }

    // calculate y-slab interval
    if (invDirection.y() > 0.0_r) {
        tmin = std::max(tmin, tnear.y());
        tmax = std::min(tmax,  tfar.y());
    }
    else {
        tmin = std::max(tmin,  tfar.y());
        tmax = std::min(tmax, tnear.y());
    }

    // calculate z-slab interval
    if (invDirection.z() > 0.0_r) {
        tmin = std::max(tmin, tnear.z());
        tmax = std::min(tmax,  tfar.z());
    }
    else {
        tmin = std::max(tmin,  tfar.z());
        tmax = std::min(tmax, tnear.z());
    }

    // check if intersection exists
    if (tmin > tmax) {
        return false;
    }

    return true;
}

template<typename T, uint32 Size>
inline Vector<T, Size> AABB<T, Size>::center() {
    return (_minVertex + _maxVertex) / static_cast<T>(2);
}

template<typename T, uint32 Size>
inline AABB<T, Size> AABB<T, Size>::unionWith(Vector<T, Size> vertex) {
    _minVertex = Vector<T, Size>::min(_minVertex, vertex);
    _maxVertex = Vector<T, Size>::max(_maxVertex, vertex);
    
    return *this;
}

template<typename T, uint32 Size>
inline AABB<T, Size> AABB<T, Size>::unionWith(AABB<T, Size> aabb) {
    _minVertex = Vector<T, Size>::min(_minVertex, aabb._minVertex);
    _maxVertex = Vector<T, Size>::max(_maxVertex, aabb._maxVertex);

    return *this;
}

} // namespace cadise