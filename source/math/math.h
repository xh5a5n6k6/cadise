#pragma once

#include "math/type/mathType.h"

#include <utility>

namespace cadise {

// some math utilities here
namespace math {

real degreeToRadian(const real degree);

real radianToDegree(const real radian);

// find the nearest perfect square number which is not greater than
// input number
std::size_t nearestLowerSquareNumber(const std::size_t number);

// find the nearest perfect square number which is not less than
// input number
std::size_t nearestUpperSquareNumber(const std::size_t number);

real fractional(const real value);

void buildCoordinateSystem(const Vector3R& zAxis, 
                           Vector3R* const out_xAxis, 
                           Vector3R* const out_yAxis);

real gammaCorrection(const real value);

real inverseGammaCorrection(const real value);

/*
    Reference Note:
    https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
*/
template<typename T>
inline int32 sign(const T& value);

template<typename T>
inline T min(const T& a, const T& b);

template<typename T>
inline T max(const T& a, const T& b);

template<typename T>
inline T clamp(const T& value, const T& lowerBound, const T& upperBound);

template<typename T>
inline void swap(T& a, T& b);

template<typename T>
inline T mapToNonZero(const T& value);

// template header implementation

template<typename T>
inline int32 sign(const T& value) {
    return (static_cast<T>(0) < value) - (value < static_cast<T>(0));
}

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
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

template<typename T>
inline T mapToNonZero(const T& value) {
    return (value == T(0)) ? T(1) : value;
}

} // namespace math

} // namespace cadise