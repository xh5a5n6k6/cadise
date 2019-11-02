#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

template<typename T, std::size_t N>
class Image;

using LdrImage      = Image<uint8, 3>;
using LdrAlphaImage = Image<uint8, 4>;
using HdrImage      = Image<real, 3>;
using HdrAlphaImage = Image<real, 4>;

} // namespace cadise