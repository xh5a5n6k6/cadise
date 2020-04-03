#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

namespace constant {

template<typename T>
inline constexpr T ray_epsilon
    = T(0.001);

// 1 degree = pi/180 radian
template<typename T>
inline constexpr T radian_per_degree
    = T(0.0174532925199432);

// 1 radian = 180/pi degree
template<typename T>
inline constexpr T degree_per_radian
    = T(57.295779513082320);

template<typename T>
inline constexpr T pi
    = T(3.1415926535897932);

template<typename T>
inline constexpr T two_pi
    = T(6.2831853071795864);

template<typename T>
inline constexpr T four_pi
    = T(12.566370614359172);

template<typename T>
inline constexpr T inv_pi
    = T(0.3183098861837906);

template<typename T>
inline constexpr T inv_two_pi
    = T(0.1591549430918953);

} // namespace constant

} // namespace cadise