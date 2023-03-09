#pragma once

#include "math/type/mathType.h"

#include <array>

namespace cadise 
{

class SampleWarp
{
public:
    static void uniformTriangleUv(
        const std::array<real, 2>& sample,
        Vector2R* const            out_uv);
};

} // namespace cadise