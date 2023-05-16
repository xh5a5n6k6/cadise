#pragma once

#include "Math/Type/MathType.h"

#include <array>

namespace cadise
{

// Reference: PBRT-v3
class Disk
{
public:
    static void uniformSampling(
        const std::array<real, 2>& sample,
        Vector2R* const            out_surfacePoint);

    static void concentricSampling(
        const std::array<real, 2>& sample,
        Vector2R* const            out_surfacePoint);
};

} // namespace cadise