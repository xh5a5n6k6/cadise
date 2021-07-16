#pragma once

#include "core/surface/microfacet/eRoughnessMapMode.h"
#include "math/math.h"

#include <cmath>

namespace cadise {

template<ERoughnessMapMode Mode>
class TRoughnessMapper {
public:
    real map(const real roughness);
};

// template header implementation

template<ERoughnessMapMode Mode>
inline real TRoughnessMapper<Mode>::map(const real roughness) {
    real alpha;
    
    const real safeRoughness = math::max(roughness, 0.001_r);
    if constexpr (Mode == ERoughnessMapMode::SQUARE) {
        alpha = safeRoughness * safeRoughness;
    }
    else if constexpr (Mode == ERoughnessMapMode::PBRT) {
        const real x = std::log(safeRoughness);

        alpha = 1.621420000_r +
                0.819955000_r * x + 
                0.173400000_r * x * x + 
                0.017120100_r * x * x * x +
                0.000640711_r * x * x * x * x;
    }
    else {
        alpha = safeRoughness;
    }

    return math::max(alpha, 0.001_r);
}

} // namespace cadise