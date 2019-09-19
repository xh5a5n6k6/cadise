#pragma once

#include "math/mathType.h"

namespace cadise {

// some math utilities here
//
namespace math {

real degreeToRadian(const real degree);

real radianToDegree(const real radian);

// find the nearest perfect square number which is not greater than
// input number
//
std::size_t nearestLowerSquareNumber(const std::size_t number);

// find the nearest perfect square number which is not less than
// input number
//
std::size_t nearestUpperSquareNumber(const std::size_t number);

void buildCoordinateSystem(const Vector3R& zAxis, Vector3R& xAxis, Vector3R& yAxis);

template<typename T>
inline T min(const T& a, const T& b);

template<typename T>
inline T max(const T& a, const T& b);

template<typename T>
inline T clamp(const T& value, const T& lowerBound, const T& upperBound);

template<typename T>
inline void swap(T& a, T& b);

// template header implementation

template<typename T>
inline T min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

template<typename T>
inline T max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

template<typename T>
inline T clamp(const T& value, const T& lowerBound, const T& upperBound) {
    return min(max(value, lowerBound), upperBound);
}

template<typename T>
inline void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

} // namespace math

} // namespace cadise