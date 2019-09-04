#pragma once

#include "core/integral-tool/lightSamplePolicy.h"
#include "core/scene.h"

#include "math/random.h"

namespace cadise {

class LightSampler {
public:
    template<LightSamplePolicy policy>
    static int32 sampleOneLight(const Scene& scene, real* const lightPdf);
};

// template header implementation

template<LightSamplePolicy policy>
inline int32 LightSampler::sampleOneLight(const Scene& scene, real* const lightPdf) {
    uint64 lightNumber = scene.lights().size();
    if constexpr (policy == LightSamplePolicy::UNIFORM) {
        int32 sampleIndex = random::get1DInt32(0, static_cast<int32>(lightNumber) - 1);

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