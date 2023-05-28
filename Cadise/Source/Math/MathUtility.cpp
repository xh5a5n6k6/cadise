#include "Math/MathUtility.h"

#include "Foundation/Assertion.h"
#include "Math/TVector2.h"
#include "Math/TVector3.h"

#include <cmath>

namespace cadise
{

real MathUtility::toRadians(const real degrees)
{
    return degrees * constant::radians_per_degree<real>;
}

real MathUtility::toDegrees(const real radians)
{
    return radians * constant::degrees_per_radian<real>;
}

std::size_t MathUtility::closestLowerSquareNumber(const std::size_t number)
{
    const auto lowerSqrtNumber = static_cast<std::size_t>(std::floor(std::sqrt(static_cast<real>(number))));

    return lowerSqrtNumber * lowerSqrtNumber;
}

std::size_t MathUtility::closestUpperSquareNumber(const std::size_t number)
{
    const auto upperSqrtNumber = static_cast<std::size_t>(std::ceil(std::sqrt(static_cast<real>(number))));

    return upperSqrtNumber * upperSqrtNumber;
}

real MathUtility::fractional(const real value)
{
    return value - std::floor(value);
}

void MathUtility::buildCoordinateSystem(
    const Vector3R& yAxis,
    Vector3R* const out_zAxis,
    Vector3R* const out_xAxis)
{
    CS_ASSERT(out_zAxis);
    CS_ASSERT(out_xAxis);

    if (std::abs(yAxis.x()) > std::abs(yAxis.y()))
    {
        Vector3R zAxis(-yAxis.z(), 0.0_r, yAxis.x());
        zAxis.divLocal(std::sqrt(yAxis.x() * yAxis.x() + yAxis.z() * yAxis.z()));

        out_zAxis->set(zAxis);
    }
    else
    {
        Vector3R zAxis(0.0_r, yAxis.z(), -yAxis.y());
        zAxis.divLocal(std::sqrt(yAxis.y() * yAxis.y() + yAxis.z() * yAxis.z()));

        out_zAxis->set(zAxis);
    }

    out_xAxis->set(yAxis.cross(*out_zAxis));
}

void MathUtility::toCanonical(const Vector3R& direction, Vector2R* const out_canonical)
{
    CS_ASSERT(out_canonical);
    CS_ASSERT(!direction.isZero());

    const Vector3R unitDirection = direction.normalize();

    const real cosTheta = MathUtility::clamp(unitDirection.y(), -1.0_r, 1.0_r);
    const real rawPhi   = std::atan2(unitDirection.x(), unitDirection.z());
    const real phi      = rawPhi < 0.0_r ? rawPhi + constant::two_pi<real> : rawPhi;

    out_canonical->set(phi * constant::rcp_two_pi<real>, (cosTheta + 1.0_r) * 0.5_r);
}

void MathUtility::toDirection(const Vector2R& canonical, Vector3R* const out_direction)
{
    CS_ASSERT(out_direction);

    const real cosTheta = canonical.y() * 2.0_r - 1.0_r;
    const real phi      = canonical.x() * constant::two_pi<real>;
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    out_direction->set(
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta);
}

real MathUtility::forwardGammaCorrection(const real value)
{
    if (value <= 0.0031308_r)
    {
        return 12.92_r * value;
    }
    else
    {
        return 1.055_r * std::pow(value, 1.0_r / 2.4_r) - 0.055_r;
    }
}

real MathUtility::inverseGammaCorrection(const real value)
{
    if (value <= 0.04045_r)
    {
        return value * 1.0_r / 12.92_r;
    }
    else
    {
        return std::pow((value + 0.055_r) / 1.055_r, 2.4_r);
    }
}

} // namespace cadise