#pragma once

#include "core/texture/textureWrapMode.h"
#include "math/type/imageType.h"
#include "math/type/mathType.h"

#include <memory>

namespace cadise {

class UvwWrapper;

template<typename T, std::size_t N>
class PixelSampler {
public:
    PixelSampler(const TextureWrapMode& mode);

    virtual ~PixelSampler();

    virtual void sample(
        const Vector3R& uvw, 
        const Image<T, N>& image, 
        Vector<T, N>* const out_value) const = 0;

protected:
    std::unique_ptr<UvwWrapper> _uvwWrapper;
};

} // namespace cadise

#include "core/texture/sampler/pixelSampler.ipp"