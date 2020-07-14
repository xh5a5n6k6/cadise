#pragma once

#include "core/texture/category/imageTexture.h"

#include "core/texture/sampler/nearestPixelSampler.h"

namespace cadise {

template<typename T, typename ImageType, std::size_t N>
ImageTexture<T, ImageType, N>::ImageTexture(
    const Image<ImageType, N>& image,
    const ETextureSampleMode&  sampleMode,
    const ETextureWrapMode&    wrapMode) :
    _image(image) {

    switch (sampleMode) {
        case ETextureSampleMode::NEAREST:
            _pixelSampler = std::make_unique<NearestPixelSampler<ImageType, N>>(wrapMode);
            break;
        default:
            _pixelSampler = std::make_unique<NearestPixelSampler<ImageType, N>>(wrapMode);
            break;
    }
}

} // namespace cadise