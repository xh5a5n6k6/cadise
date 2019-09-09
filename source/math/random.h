#pragma once

#include "math/fundamentalType.h"

namespace cadise {

namespace random {

real nextReal();

std::size_t nextIndex(const std::size_t minIndex, const std::size_t maxIndex);

} // namespace random

} // namespace cadise