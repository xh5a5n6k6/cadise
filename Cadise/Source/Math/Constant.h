#pragma once

#include <type_traits>

namespace cadise::constant
{

template<typename T>
inline constexpr T ray_epsilon
    = static_cast<T>(0.001);

// 1 degree = pi/180 radian
template<typename T>
inline constexpr T radian_per_degree
    = static_cast<T>(0.01745329251994329576);

// 1 radian = 180/pi degree
template<typename T>
inline constexpr T degree_per_radian
    = static_cast<T>(57.2957795130823208767);

template<typename T>
inline constexpr T pi
    = static_cast<T>(3.14159265358979323846);

template<typename T>
inline constexpr T two_pi
    = static_cast<T>(6.28318530717958647692);

template<typename T>
inline constexpr T four_pi
    = static_cast<T>(12.5663706143591729538);

template<typename T>
inline constexpr T rcp_pi
    = static_cast<T>(0.31830988618379067153);

template<typename T>
inline constexpr T rcp_two_pi
    = static_cast<T>(0.15915494309189533576);

template<typename T>
inline constexpr T rcp_four_pi
    = static_cast<T>(0.07957747154594766788);

template<typename T>
inline constexpr T pi_over_two
    = static_cast<T>(1.57079632679489661923);

template<typename T>
inline constexpr T pi_over_four
    = static_cast<T>(0.78539816339744830961);



using SignType = int;

static_assert(std::is_signed_v<SignType>,
    "Sign type needs to be signed.\n");

inline constexpr SignType SIGN_POSITIVE =  1;
inline constexpr SignType SIGN_ZERO     =  0;
inline constexpr SignType SIGN_NEGATIVE = -1;



using AxisType = int;

inline constexpr AxisType AXIS_X       =  0;
inline constexpr AxisType AXIS_Y       =  1;
inline constexpr AxisType AXIS_Z       =  2;
inline constexpr AxisType AXIS_UNKNOWN = -1;

/*
    +-------+-------+-------+-------------+------+
    | x < y | x < z | y < z |    result   | axis |
    +-------+-------+-------+-------------+------+
    |   0   |   0   |   0   | x >= y >= z |   x  |
    +-------+-------+-------+-------------+------+
    |   0   |   0   |   1   |  x >= z > y |   x  |
    +-------+-------+-------+-------------+------+
    |   0   |   1   |   0   |      -      |   -  |
    +-------+-------+-------+-------------+------+
    |   0   |   1   |   1   |  z > x >= y |   z  |
    +-------+-------+-------+-------------+------+
    |   1   |   0   |   0   |  y > x >= z |   y  |
    +-------+-------+-------+-------------+------+
    |   1   |   0   |   1   |      -      |   -  |
    +-------+-------+-------+-------------+------+
    |   1   |   1   |   0   |  y >= z > x |   y  |
    +-------+-------+-------+-------------+------+
    |   1   |   1   |   1   |   z > y > x |   z  |
    +-------+-------+-------+-------------+------+
*/
inline constexpr AxisType compareMaxTable[8] =
{
    AXIS_X,
    AXIS_X,
    AXIS_UNKNOWN,
    AXIS_Z,
    AXIS_Y,
    AXIS_UNKNOWN,
    AXIS_Y,
    AXIS_Z,
};

} // namespace cadise::constant