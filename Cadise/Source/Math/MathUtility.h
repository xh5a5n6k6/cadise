#pragma once

#include "Math/Constant.h"
#include "Math/Type/MathType.h"

#include <algorithm>
#include <cmath>

namespace cadise
{

/*! Miscellaneous math utilities. */
class MathUtility final
{
public:
    /*
    Retrieve the sign of the input value, which can be positive, zero, or negative.

    Reference:
    https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
    */
    template<typename T>
    static constant::SignType sign(const T& value);

    /*! Return the square of the input value. */
    template<typename T>
    static T squared(const T& value);

    /*!
    Return the lowerBound if the input value is NaN;
    otherwise, return the normal clamped result.
    */
    template<typename T>
    static T clamp(const T& value, const T& lowerBound, const T& upperBound);

    /*! Return one if the input value is zero; otherwise, return the input value. */
    template<typename T>
    static T mapZeroToOne(const T& value);

    /*! Convert the input degrees to radians. */
    static real toRadians(const real degrees);

    /*! Convert the input radians to degrees. */
    static real toDegrees(const real radians);

    /*!
    Find the closest perfect square number that is not greater than the input number.
    */
    static std::size_t closestLowerSquareNumber(const std::size_t number);

    /*!
    Find the closest perfect square number that is not less than the input number.
    */
    static std::size_t closestUpperSquareNumber(const std::size_t number);

    /*! Extract the fractional part of the input value. */
    static real fractional(const real value);

    /*! Construct a local coordinate system using the given y-axis (up vector). */
    static void buildCoordinateSystem(
        const Vector3R& yAxis,
        Vector3R* const out_zAxis,
        Vector3R* const out_xAxis);

    /*!
    Convert the input direction (x, y, z) to the canonical coordinates (u, v).

    u is in [0, 1], representing phi in [0, 2pi]
    v is in [0, 1], representing cosTheta in [-1, 1]
    */
    static void toCanonical(
        const Vector3R& direction,
        Vector2R* const out_canonical);

    /*!
    Convert the input canonical coordinates (u, v) to the direction (x, y, z).

    u is in [0, 1], representing phi in [0, 2pi]
    v is in [0, 1], representing cosTheta in [-1, 1]
    */
    static void toDirection(
        const Vector2R& canonical,
        Vector3R* const out_direction);

    /*! Map the input value from linear-sRGB color space to sRGB color space. */
    static real forwardGammaCorrection(const real value);

    /*! Map the input value from sRGB color space to linear-sRGB color space. */
    static real inverseGammaCorrection(const real value);
};

// template header implementation

template <typename T>
inline constant::SignType MathUtility::sign(const T& value)
{
    return static_cast<constant::SignType>(
        (static_cast<T>(0) < value) - (value < static_cast<T>(0)));
}

template <typename T>
inline T MathUtility::squared(const T& value)
{
    return value * value;
}

template <typename T>
inline T MathUtility::clamp(const T& value, const T& lowerBound, const T& upperBound)
{
    return std::isnan(value) ? lowerBound : std::clamp(value, lowerBound, upperBound);
}

template <typename T>
inline T MathUtility::mapZeroToOne(const T& value)
{
    return value == static_cast<T>(0) ? static_cast<T>(1) : value;
}

} // namespace cadise