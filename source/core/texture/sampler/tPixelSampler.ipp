#pragma once

#include "core/texture/sampler/tPixelSampler.h"

#include "core/texture/wrapper/clampUvwWrapper.h"
#include "core/texture/wrapper/repeatUvwWrapper.h"
#include "fundamental/assertion.h"

namespace cadise {

template<typename T, std::size_t N>
TPixelSampler<T, N>::TPixelSampler(const ETextureWrapMode& mode) {
    switch (mode) {
        case ETextureWrapMode::CLAMP:
            _uvwWrapper = std::make_unique<ClampUvwWrapper>();
            break;
        case ETextureWrapMode::REPEAT:
            _uvwWrapper = std::make_unique<RepeatUvwWrapper>();
            break;
        default:
            _uvwWrapper = std::make_unique<RepeatUvwWrapper>();
            break;
    }
}

template<typename T, std::size_t N>
TPixelSampler<T, N>::~TPixelSampler() = default;

} // namespace cadise