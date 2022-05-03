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
inline constexpr T rcp_pi
    = static_cast<T>(0.3183098861837906);

template<typename T>
inline constexpr T rcp_two_pi
    = static_cast<T>(0.1591549430918953);

template<typename T>
inline constexpr T rcp_four_pi
    = static_cast<T>(0.0795774715459476);



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
inline constexpr constant::AxisType compareMaxTable[8] = 
{
    constant::AXIS_X,
    constant::AXIS_X,
    constant::AXIS_UNKNOWN,
    constant::AXIS_Z,
    constant::AXIS_Y,
    constant::AXIS_UNKNOWN,
    constant::AXIS_Y,
    constant::AXIS_Z 
};

} // namespace cadise::constant