#include "math/math.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

namespace math {

real degree_to_radian(const real degree) {
    return degree * constant::radian_per_degree<real>;
}

real radian_to_degree(const real radian) {
    return radian * constant::degree_per_radian<real>;
}

std::size_t nearest_lower_square_number(const std::size_t number) {
    const std::size_t lowerSqrtNumber
        = static_cast<std::size_t>(std::floor(std::sqrt(static_cast<real>(number))));

    return lowerSqrtNumber * lowerSqrtNumber;
}

std::size_t nearest_upper_square_number(const std::size_t number) {
    const std::size_t upperSqrtNumber
        = static_cast<std::size_t>(std::ceil(std::sqrt(static_cast<real>(number))));

    return upperSqrtNumber * upperSqrtNumber;
}

real fractional(const real value) {
    return value - std::floor(value);
}

void build_coordinate_system(
    const Vector3R& zAxis, 
    Vector3R* const out_xAxis, 
    Vector3R* const out_yAxis) {
    
    CADISE_ASSERT(out_xAxis);
    CADISE_ASSERT(out_yAxis);

    if (std::abs(zAxis.x()) > std::abs(zAxis.y())) {
        Vector3R xAxis(-zAxis.z(), 0.0_r, zAxis.x());
        xAxis /= std::sqrt(zAxis.x() * zAxis.x() + zAxis.z() * zAxis.z());

        *out_xAxis = xAxis;
    }
    else {
        Vector3R xAxis(0.0_r, zAxis.z(), -zAxis.y());
        xAxis /= std::sqrt(zAxis.y() * zAxis.y() + zAxis.z() * zAxis.z());

        *out_xAxis = xAxis;
    }

    *out_yAxis = zAxis.cross(*out_xAxis);
}

real gamma_correction(const real value) {
    if (value <= 0.0031308_r) {
        return 12.92_r * value;
    }
    else {
        return 1.055_r * std::pow(value, 1.0_r / 2.4_r) - 0.055_r;
    }
}

real inverse_gamma_correction(const real value) {
    if (value <= 0.04045_r) {
        return value * 1.0_r / 12.92_r;
    }
    else {
        return std::pow((value + 0.055_r) / 1.055_r, 2.4_r);
    }
}

} // namespace math

} // namespace cadise