#pragma once

#include "core/texture/sampler/nearestPixelSampler.h"

#include "core/imaging/image.h"
#include "core/texture/wrapper/uvwWrapper.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/vector.h"

namespace cadise {

template<typename T, std::size_t N>
inline void NearestPixelSampler<T, N>::sample(
    const Vector3R& uvw,
    const Image<T, N>& image,
    Vector<T, N>* const out_value) const {

    CADISE_ASSERT(out_value);

    Vector3R wrapUvw;
    _uvwToWrapUvw(uvw, &wrapUvw);
    
    int32 sampleW = static_cast<int32>(wrapUvw.x() * image.width());
    int32 sampleH = static_cast<int32>(wrapUvw.y() * image.height());

    sampleW = (sampleW < image.width())  ? sampleW : sampleW - 1;
    sampleH = (sampleH < image.height()) ? sampleH : sampleH - 1;

    image.getImagePixel(sampleW, sampleH, out_value);
}

} // namespace cadise