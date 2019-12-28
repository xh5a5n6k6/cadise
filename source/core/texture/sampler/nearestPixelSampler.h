#pragma once

#include "core/texture/sampler/pixelSampler.h"

namespace cadise {

template<typename T, std::size_t N>
class NearestPixelSampler : public PixelSampler<T, N> {
public:
    using PixelSampler::PixelSampler;
    
    void sample(
        const Vector3R& uvw,
        const Image<T, N>& image,
        Vector<T, N>* const out_value) const override;
};

} // namespace cadise

#include "core/texture/sampler/nearestPixelSampler.ipp"