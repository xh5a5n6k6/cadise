#pragma once

#include "math/vector.h"

namespace cadise {

namespace sample {

Vector3R uniformHemisphere(const Vector2R randomNumber);

Vector3R cosineWeightedHemisphere(const Vector2R randomNumber);

} // namespace sample

} // namespace cadise