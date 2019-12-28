#pragma once

#include "core/texture/sampler/pixelSampler.h"

#include "core/texture/wrapper/clampUvwWrapper.h"
#include "core/texture/wrapper/repeatUvwWrapper.h"
#include "fundamental/assertion.h"

namespace cadise {

template<typename T, std::size_t N>
PixelSampler<T, N>::PixelSampler(const TextureWrapMode& mode) {
    switch (mode) {
        case TextureWrapMode::CLAMP:
            _uvwWrapper = std::make_unique<ClampUvwWrapper>();
            break;
        case TextureWrapMode::REPEAT:
            _uvwWrapper = std::make_unique<RepeatUvwWrapper>();
            break;
        default:
            _uvwWrapper = std::make_unique<RepeatUvwWrapper>();
            break;
    }
}

template<typename T, std::size_t N>
void PixelSampler<T, N>::_uvwToWrapUvw(const Vector3R& uvw, Vector3R* const out_wrapUvw) const {
    CADISE_ASSERT(out_wrapUvw);

    _uvwWrapper->wrap(uvw, out_wrapUvw);
}

} // namespace cadise