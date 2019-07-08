#pragma once

#include "math/vector.h"

namespace cadise {

namespace math {

real degreeToRadian(const real degree);

real radianToDegree(const real radian);

void buildCoordinateSystem(const Vector3R& zAxis, Vector3R& xAxis, Vector3R& yAxis);

} // namespace math

} // namespace cadise