#pragma once

#include "math/type/mathType.h"

#include <array>

namespace cadise 
{

class Hemisphere 
{
public:
    static void uniformSampling(
        const std::array<real, 2>& sample, 
        Vector3R* const            out_direction,
        real* const                out_pdfW);

    static void cosineWeightedSampling(
        const std::array<real, 2>& sample,
        Vector3R* const            out_direction,
        real* const                out_pdfW);

    static void cosineExpWeightedSampling(
        const std::array<real, 2>& sample,
        const real                 exponent,                                          
        Vector3R* const            out_direction,
        real* const                out_pdfW);
};

} // namespace cadise