#pragma once

#include "math/mathType.h"

namespace cadise {

namespace hemisphere {

Vector3R uniformSampling(const Vector2R& randomNumber);

Vector3R cosineWeightedSampling(const Vector2R& randomNumber);

} // namespace hemisphere

} // namespace cadise