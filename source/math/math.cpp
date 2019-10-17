#include "math/math.h"

#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

namespace math {

real degreeToRadian(const real degree) {
    return degree * constant::RADIAN_PER_DEGREE;
}

real radianToDegree(const real radian) {
    return radian * constant::DEGREE_PER_RADIAN;
}

std::size_t nearestLowerSquareNumber(const std::size_t number) {
    std::size_t lowerSqrtNumber
        = static_cast<std::size_t>(std::floor(std::sqrt(static_cast<real>(number))));

    return lowerSqrtNumber * lowerSqrtNumber;
}

std::size_t nearestUpperSquareNumber(const std::size_t number) {
    std::size_t upperSqrtNumber
        = static_cast<std::size_t>(std::ceil(std::sqrt(static_cast<real>(number))));

    return upperSqrtNumber * upperSqrtNumber;
}

void buildCoordinateSystem(const Vector3R& zAxis, Vector3R& xAxis, Vector3R& yAxis) {
    if (std::abs(zAxis.x()) > std::abs(zAxis.y())) {
        xAxis = Vector3R(-zAxis.z(), 0.0_r, zAxis.x()) / std::sqrt(zAxis.x() * zAxis.x() + zAxis.z() * zAxis.z());
    }
    else {
        xAxis = Vector3R(0.0_r, zAxis.z(), -zAxis.y()) / std::sqrt(zAxis.y() * zAxis.y() + zAxis.z() * zAxis.z());
    }

    yAxis = zAxis.cross(xAxis);
}

real gammaCorrection(real value) {
    if (value <= 0.0031308_r) {
        return 12.92_r * value;
    }

    return 1.055_r * std::pow(value, 1._r / 2.4_r) - 0.055_r;
}

} // namespace math

} // namespace cadise