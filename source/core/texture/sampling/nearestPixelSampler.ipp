#pragma once

#include "core/texture/sampling/nearestPixelSampler.h"

#include "core/imaging/image.h"

#include "math/math.h"
#include "math/vector.h"

namespace cadise {

template<typename T, std::size_t N>
inline void NearestPixelSampler<T, N>::sample(
    const Vector3R& uvw,
    const Image<T, N>& image,
    Vector<T, N>* const out_value) const {

    // clamp uv to [0.0, 1.0]
    const real u = math::clamp(uvw.x(), 0.0_r, 1.0_r);
    const real v = math::clamp(uvw.y(), 0.0_r, 1.0_r);
    
    int32 sampleW = static_cast<int32>(u * image.width());
    int32 sampleH = static_cast<int32>(v * image.height());

    sampleW = (sampleW < image.width())  ? sampleW : sampleW - 1;
    sampleH = (sampleH < image.height()) ? sampleH : sampleH - 1;

    image.getImagePixel(sampleW, sampleH, out_value);
}

} // namespace cadise