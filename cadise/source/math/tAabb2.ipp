#pragma once

#include "math/tAabb2.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <limits>
#include <type_traits>

namespace cadise {

template<typename T>
inline TAABB2<T>::TAABB2() :
    TAABB2(TVector2<T>(std::numeric_limits<T>::max()), 
           TVector2<T>(std::numeric_limits<T>::min())) {
}

template<typename T>
inline TAABB2<T>::TAABB2(const TVector2<T>& vertex) :
    TAABB2(vertex, vertex) {
}

template<typename T>
inline TAABB2<T>::TAABB2(const TVector2<T>& minVertex, const TVector2<T>& maxVertex) :
    _minVertex(minVertex),
    _maxVertex(maxVertex) { 
}

template<typename T>
inline bool TAABB2<T>::isEmpty() const {
    return _minVertex.isEqualTo(TVector2<T>(std::numeric_limits<T>::max())) ||
           _maxVertex.isEqualTo(TVector2<T>(std::numeric_limits<T>::min()));
}

template<typename T>
inline T TAABB2<T>::area() const {
    return this->extent().product();
}

template<typename T>
inline TVector2<T> TAABB2<T>::extent() const {
    return _maxVertex.sub(_minVertex);
}

template<typename T>
inline constant::AxisType TAABB2<T>::maxAxis() const {
    return this->extent().maxDimension();
}

template<typename T>
inline const TVector2<T>& TAABB2<T>::minVertex() const {
    return _minVertex;
}

template<typename T>
inline const TVector2<T>& TAABB2<T>::maxVertex() const {
    return _maxVertex;
}

template<typename T>
void TAABB2<T>::setMinVertex(const TVector2<T>& minVertex) {
    _minVertex = minVertex;
}

template<typename T>
void TAABB2<T>::setMaxVertex(const TVector2<T>& maxVertex) {
    _maxVertex = maxVertex;
}

template<typename T>
void TAABB2<T>::setMinMaxVertices(
    const TVector2<T>& minVertex,
    const TVector2<T>& maxVertex) {

    this->setMinVertex(minVertex);
    this->setMaxVertex(maxVertex);
}

template<typename T>
void TAABB2<T>::set(const TAABB2<T>& other) {
    _minVertex = other._minVertex;
    _maxVertex = other._maxVertex;
}

} // namespace cadise