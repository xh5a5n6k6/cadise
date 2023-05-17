#pragma once

#include "Math/Type/MathType.h"

#include <array>

namespace cadise
{

class Hemisphere
{
public:
    static Vector3R uniformSampling(const std::array<real, 2>& sample);

    static Vector3R uniformSampling(
        const std::array<real, 2>& sample,
        real* const                out_pdfW);

    static Vector3R cosineWeightedSampling(const std::array<real, 2>& sample);

    static Vector3R cosineWeightedSampling(
        const std::array<real, 2>& sample,
        real* const                out_pdfW);

    static Vector3R cosineExpWeightedSampling(
        const std::array<real, 2>& sample,
        const real                 exponent);

    static Vector3R cosineExpWeightedSampling(
        const std::array<real, 2>& sample,
        const real                 exponent,
        real* const                out_pdfW);
};

} // namespace cadise