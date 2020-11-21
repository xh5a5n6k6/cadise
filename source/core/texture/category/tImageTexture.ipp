#pragma once

#include "core/texture/category/tImageTexture.h"

#include "core/texture/sampler/tNearestPixelSampler.h"

namespace cadise {

template<typename T, typename ImageType, std::size_t N>
TImageTexture<T, ImageType, N>::TImageTexture(
    const TImage<ImageType, N>& image,
    const ETextureSampleMode&   sampleMode,
    const ETextureWrapMode&     wrapMode) :
    _image(image) {

    switch (sampleMode) {
        case ETextureSampleMode::NEAREST:
            _pixelSampler = std::make_unique<TNearestPixelSampler<ImageType, N>>(wrapMode);
            break;
        default:
            _pixelSampler = std::make_unique<TNearestPixelSampler<ImageType, N>>(wrapMode);
            break;
    }
}

} // namespace cadise