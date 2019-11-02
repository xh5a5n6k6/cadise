#pragma once

#include "core/texture/category/imageTexture.h"

#include "core/texture/sampling/nearestPixelSampler.h"

namespace cadise {

template<typename T, typename ImageType, std::size_t N>
ImageTexture<T, ImageType, N>::ImageTexture(const Image<ImageType, N>& image,
                                            const TextureSamplingMode& mode) :
    _image(image) {

    switch (mode) {
        case TextureSamplingMode::NEAREST:
            _pixelSampler = std::make_unique<NearestPixelSampler<ImageType, N>>();
            break;
        default:
            _pixelSampler = std::make_unique<NearestPixelSampler<ImageType, N>>();
            break;
    }
}

} // namespace cadise