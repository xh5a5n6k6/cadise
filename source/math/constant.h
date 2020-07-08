#pragma once

#include <type_traits>

namespace cadise {

namespace constant {

template<typename T>
inline constexpr T ray_epsilon
    = static_cast<T>(0.001);

// 1 degree = pi/180 radian
template<typename T>
inline constexpr T radian_per_degree
    = static_cast<T>(0.0174532925199432);

// 1 radian = 180/pi degree
template<typename T>
inline constexpr T degree_per_radian
    = static_cast<T>(57.295779513082320);

template<typename T>
inline constexpr T pi
    = static_cast<T>(3.1415926535897932);

template<typename T>
inline constexpr T two_pi
    = static_cast<T>(6.2831853071795864);

template<typename T>
inline constexpr T four_pi
    = static_cast<T>(12.566370614359172);

template<typename T>
inline constexpr T inv_pi
    = static_cast<T>(0.3183098861837906);

template<typename T>
inline constexpr T inv_two_pi
    = static_cast<T>(0.1591549430918953);

template<typename T>
inline constexpr T inv_four_pi
    = static_cast<T>(0.0795774715459476);



using Sign = int;

static_assert(std::is_signed_v<Sign>, 
    "Sign type needs to be signed.\n");

inline constexpr Sign SIGN_POSITIVE =  1;
inline constexpr Sign SIGN_ZERO     =  0;
inline constexpr Sign SIGN_NEGATIVE = -1;

} // namespace constant

} // namespace cadise