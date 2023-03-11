#pragma once

#include "Math/Type/PrimitiveType.h"

namespace cadise 
{

template<typename T, std::size_t N>
class TImage;

using LDRImage      = TImage<uint8, 3>;
using LDRAlphaImage = TImage<uint8, 4>;
using HDRImage      = TImage<real, 3>;
using HDRAlphaImage = TImage<real, 4>;

} // namespace cadise