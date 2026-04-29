#pragma once

#include "Math/Type/MathType.h"

#include <array>

namespace cadise
{

// Reference: PBRT-v3
class Disk
{
public:
    static Vector2R uniformSampling(const std::array<real, 2>& sample);

    static Vector2R concentricSampling(const std::array<real, 2>& sample);
};

} // namespace cadise