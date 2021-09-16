#pragma once

#include "core/texture/category/tImageTexture.h"

#include "core/texture/sampler/tNearestPixelSampler.h"

namespace cadise 
{

template<typename T, typename ImageType, std::size_t N>
inline TImageTexture<T, ImageType, N>::TImageTexture(
    const TImage<ImageType, N>& image,
    const ETextureSampleMode    sampleMode,
    const ETextureWrapMode      wrapMode) :

    _pixelSampler(nullptr) 
{
    switch (sampleMode) 
    {
        case ETextureSampleMode::NEAREST:
            _pixelSampler = std::make_unique<TNearestPixelSampler<ImageType, N>>(image, wrapMode);
            break;

        default:
            _pixelSampler = std::make_unique<TNearestPixelSampler<ImageType, N>>(image, wrapMode);
            break;
    }
}

} // namespace cadise