#pragma once

#include "math/type/mathType.h"

namespace cadise {

namespace hemisphere {

void uniformSampling(const Vector2R& randomNumber, Vector3R* const out_direction);

void cosineWeightedSampling(const Vector2R& randomNumber, Vector3R* const out_direction);

} // namespace hemisphere

} // namespace cadise