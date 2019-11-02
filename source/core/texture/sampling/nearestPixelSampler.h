#pragma once

#include "core/texture/sampling/pixelSampler.h"

namespace cadise {

template<typename T, std::size_t N>
class NearestPixelSampler : public PixelSampler<T, N> {
public:
    void sample(
        const Vector3R& uvw,
        const Image<T, N>& image,
        Vector<T, N>* const out_value) const override;
};

} // namespace cadise

#include "core/texture/sampling/nearestPixelSampler.ipp"