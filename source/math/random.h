#pragma once

#include "math/vector.h"

namespace cadise {

namespace random {

real get1D();

Vector2R get2D();

int32 get1DInt32(const int32 minNumber, const int32 maxNumber);

} // namespace random

} // namespace cadise