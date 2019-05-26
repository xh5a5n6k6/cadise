#pragma once

#include "math/aabb.h"

#include <limits>

namespace cadise {

template<typename T, uint64 Size>
inline AABB<T, Size>::AABB<T, Size>() :
    _minVertex(std::numeric_limits<T>::max()),
    _maxVertex(std::numeric_limits<T>::min()) { 
}

template<typename T, uint64 Size>
inline AABB<T, Size>::AABB<T, Size>(Vector<T, Size> vertex) :
    _minVertex(vertex), _maxVertex(vertex) {
}

template<typename T, uint64 Size>
inline AABB<T, Size>::AABB<T, Size>(Vector<T, Size> minVertex, Vector<T, Size> maxVertex) :
    _minVertex(minVertex), _maxVertex(maxVertex) {
}

template<typename T, uint64 Size>
inline bool AABB<T, Size>::isOccluded() {
    static_assert(Size == 3, "Error in AABB's isOccluded, this AABB doesn't support isOccluded\n");

    return true;
}

template<typename T, uint64 Size>
inline Vector<T, Size> AABB<T, Size>::center() {
    return (_minVertex + _maxVertex) / static_cast<T>(2);
}

template<typename T, uint64 Size>
inline AABB<T, Size> AABB<T, Size>::unionWith(Vector<T, Size> vertex) {
    return AABB<T, Size>(Vector<T, Size>::min(_minVertex, vertex),
                         Vector<T, Size>::max(_maxVertex, vertex));
}

template<typename T, uint64 Size>
inline AABB<T, Size> AABB<T, Size>::unionWith(AABB<T, Size> aabb) {
    return AABB<T, Size>(Vector<T, Size>::min(_minVertex, aabb._minVertex),
                         Vector<T, Size>::max(_maxVertex, aabb._maxVertex));
}

} // namespace cadise