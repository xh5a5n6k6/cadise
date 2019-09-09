#pragma once

#include "core/integral-tool/lightSamplePolicy.h"
#include "core/scene.h"

#include "math/random.h"

namespace cadise {

class LightSampler {
public:
    template<LightSamplePolicy policy>
    static std::size_t sampleOneLight(const Scene& scene, real* const lightPdf);
};

// template header implementation

template<LightSamplePolicy policy>
inline std::size_t LightSampler::sampleOneLight(const Scene& scene, real* const lightPdf) {
    std::size_t lightNumber = scene.lights().size();
    if constexpr (policy == LightSamplePolicy::UNIFORM) {
        std::size_t sampleIndex = random::nextIndex(0, lightNumber - 1);

        *lightPdf = 1.0_r / static_cast<real>(lightNumber);

        return sampleIndex;
    }
    
    if constexpr (policy == LightSamplePolicy::IMPORTANCE) {
        // TODO
    }

    // go wrong
    return -1;
}

} // namespace cadise