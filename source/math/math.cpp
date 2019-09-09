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

void buildCoordinateSystem(const Vector3R& zAxis, Vector3R& xAxis, Vector3R& yAxis) {
    if (std::abs(zAxis.x()) > std::abs(zAxis.y())) {
        xAxis = Vector3R(-zAxis.z(), 0.0_r, zAxis.x()) / std::sqrt(zAxis.x() * zAxis.x() + zAxis.z() * zAxis.z());
    }
    else {
        xAxis = Vector3R(0.0_r, zAxis.z(), -zAxis.y()) / std::sqrt(zAxis.y() * zAxis.y() + zAxis.z() * zAxis.z());
    }

    yAxis = zAxis.cross(xAxis);
}

} // namespace math

} // namespace cadise