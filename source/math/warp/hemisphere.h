#pragma once

#include "math/type/mathType.h"

namespace cadise {

class Hemisphere {
public:
    static void uniformSampling(
        const Vector2R& sample, 
        Vector3R* const out_direction);

    static void cosineWeightedSampling(
        const Vector2R& sample, 
        Vector3R* const out_direction);

    static void cosineExpWeightedSampling(
        const Vector2R& sample,
        const real      exponent,                                          
        Vector3R* const out_direction);
};

} // namespace cadise