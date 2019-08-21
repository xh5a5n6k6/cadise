#pragma once

#include "core/integral-tool/lightSamplePolicy.h"

#include "math/random.h"

namespace cadise {

class Scene;

class LightSampler {
public:
    template<LightSamplePolicy policy>
    static int32 sampleOneLight(const Scene& scene, real* const lightPdf);
};

// template header implementation

template<LightSamplePolicy policy>
inline int32 LightSampler::sampleOneLight(const Scene& scene, real* const lightPdf) {
    uint64 lightNumber = scene.lights().size();
    if (policy == LightSamplePolicy::UNIFORM) {
        int32 sampleIndex = random::get1DInt32(0, static_cast<int32>(lightNumber) - 1);

        *lightPdf = 1.0_r / static_cast<real>(lightNumber);

        return sampleIndex;
    }
    else if (policy == LightSamplePolicy::IMPORTANCE) {
        // TODO
    }
    else {
        // go wrong
        return -1;
    }
}

} // namespace cadise