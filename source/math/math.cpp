#include "math/math.h"

#include <cmath>

namespace cadise {

namespace math {

void buildCoordinateSystem(const Vector3R &zAxis, Vector3R &xAxis, Vector3R &yAxis) {
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