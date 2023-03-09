#pragma once

#include "core/texture/sampler/tPixelSampler.h"

#include "core/texture/wrapper/clampUvwWrapper.h"
#include "core/texture/wrapper/repeatUvwWrapper.h"
#include "fundamental/assertion.h"

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
            _uvwWrapper = std::make_unique<ClampUvwWrapper>();
            break;

        case ETextureWrapMode::Repeat:
            _uvwWrapper = std::make_unique<RepeatUvwWrapper>();
            break;

        default:
            _uvwWrapper = std::make_unique<RepeatUvwWrapper>();
            break;
    }
}

template<typename T, std::size_t N>
inline TPixelSampler<T, N>::~TPixelSampler() = default;

} // namespace cadise