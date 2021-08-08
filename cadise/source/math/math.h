#pragma once

#include "math/constant.h"
#include "math/type/mathType.h"

#include <algorithm>
#include <cmath>

/*
    some math utilities here
*/
namespace cadise::math {

/*
    map degree to radian
*/
real degree_to_radian(const real degree);

/*
    map radian to degree
*/
real radian_to_degree(const real radian);

/*
    find the nearest perfect square number which is not greater than
    input number
*/
std::size_t nearest_lower_square_number(const std::size_t number);

/*
    find the nearest perfect square number which is not less than
    input number
*/
std::size_t nearest_upper_square_number(const std::size_t number);

/*
    extract the fractional part of a floating point
*/
real fractional(const real value);

/*
    given up-axis, build local coordinate system
*/
void build_coordinate_system(
    const Vector3R& yAxis, 
    Vector3R* const out_zAxis, 
    Vector3R* const out_xAxis);

/*
    direction (x, y, z) maps to canonical (u, v)

    u: [0, 1] represents phi      [0, 2pi]
    v: [0, 1] represents cosTheta [-1, 1]
*/
void direction_to_canonical(
    const Vector3R& direction,
    Vector2R* const out_canonical);

/*
    canonical (u, v) maps to direction (x, y, z)

    u: [0, 1] represents phi      [0, 2pi]
    v: [0, 1] represents cosTheta [-1, 1]
*/
void canonical_to_direction(
    const Vector2R& canonical,
    Vector3R* const out_direction);

/*
    linear value maps to standard (non-linear) value
*/
real forward_gamma_correction(const real value);

/*
    standard (non-linear) value maps to linear value
*/
real inverse_gamma_correction(const real value);

/*
    get the sign of the input value, and there are
    three situations: positive, zero, or negative

    Reference Note:
    https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
*/
template<typename T>
inline constant::SignType sign(const T& value) {
    return static_cast<constant::SignType>(
        (static_cast<T>(0) < value) - (value < static_cast<T>(0)));
}

template<typename T>
inline T squared(const T& value) {
    return value * value;
}

/*
    if value is NaN, return lowerBound, otherwise return normal clamp result
*/
template<typename T>
inline T clamp(const T& value, const T& lowerBound, const T& upperBound) {
    return std::isnan(value) ? lowerBound : std::clamp(value, lowerBound, upperBound);
}

/*
    zero maps to one and the others remain the same
*/
template<typename T>
inline T map_to_non_zero(const T& value) {
    return (value == static_cast<T>(0)) ? static_cast<T>(1) : value;
}

} // namespace cadise::math