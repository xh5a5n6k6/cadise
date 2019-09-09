#pragma once

#include "math/fundamentalType.h"

namespace cadise {

namespace constant {

inline constexpr real RAY_EPSILON
    = 0.01_r;

// 1 degree = pi/180 radian
inline constexpr real RADIAN_PER_DEGREE
    = 0.0174532925199432_r;

// 1 radian = 180/pi degree
inline constexpr real DEGREE_PER_RADIAN
    = 57.295779513082320_r;

inline constexpr real PI
    = 3.1415926535897932_r;

inline constexpr real TWO_PI
    = 6.2831853071795864_r;

inline constexpr real FOUR_PI
    = 12.566370614359172_r;

inline constexpr real INV_PI
    = 0.3183098861837906_r;

inline constexpr real INV_TWO_PI
    = 0.1591549430918953_r;

} // namespace constant

} // namespace cadise