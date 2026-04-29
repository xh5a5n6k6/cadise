#pragma once

#include "Core/Texture/Sampler/TPixelSampler.h"

#include "Core/Texture/Wrapper/ClampUVWWrapper.h"
#include "Core/Texture/Wrapper/RepeatUVWWrapper.h"

namespace cadise
{

template<typename T, std::size_t N>
inline TPixelSampler<T, N>::TPixelSampler(
    const TImage<T, N>&    image,
    const ETextureWrapMode mode) :

    _image(image),
    _uvwWrapper(nullptr)
{
    switch (mode)
    {
        case ETextureWrapMode::Clamp:
            _uvwWrapper = std::make_unique<ClampUVWWrapper>();
            break;

        case ETextureWrapMode::Repeat:
            _uvwWrapper = std::make_unique<RepeatUVWWrapper>();
            break;

        default:
            _uvwWrapper = std::make_unique<RepeatUVWWrapper>();
            break;
    }
}

template<typename T, std::size_t N>
inline TPixelSampler<T, N>::~TPixelSampler() = default;

} // namespace cadise