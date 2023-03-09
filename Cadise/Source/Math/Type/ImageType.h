#pragma once

#include "math/type/fundamentalType.h"

namespace cadise 
{

template<typename T, std::size_t N>
class TImage;

using LdrImage      = TImage<uint8, 3>;
using LdrAlphaImage = TImage<uint8, 4>;
using HdrImage      = TImage<real, 3>;
using HdrAlphaImage = TImage<real, 4>;

} // namespace cadise