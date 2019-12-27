#pragma once

#include "core/integral-tool/lightSamplePolicy.h"
#include "core/scene.h"
#include "fundamental/assertion.h"
#include "math/random.h"

namespace cadise {

class LightSampler {
public:
    template<LightSamplePolicy Policy>
    static std::size_t sampleOneLight(
        const std::vector<std::shared_ptr<Light>>& lights, 
        real* const out_lightPdf);
};

// template header implementation

template<LightSamplePolicy Policy>
inline std::size_t LightSampler::sampleOneLight(
    const std::vector<std::shared_ptr<Light>>& lights, 
    real* const out_lightPdf) {

    CADISE_ASSERT(!lights.empty());

    const std::size_t lightNumber = lights.size();
    if constexpr (Policy == LightSamplePolicy::UNIFORM) {
        const std::size_t sampleIndex = Random::nextIndex(0, lightNumber);

        *out_lightPdf = 1.0_r / static_cast<real>(lightNumber);

        return sampleIndex;
    }
    
    // TODO: 
    //if constexpr (policy == LightSamplePolicy::IMPORTANCE) {
    //    
    //}

    // go wrong
    return -1;
}

} // namespace cadise