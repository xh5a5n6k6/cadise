#pragma once

#include "core/texture/texture.h"

#include "core/imaging/image.h"
#include "core/texture/textureSamplingMode.h"

#include <memory>

namespace cadise {

template<typename T, std::size_t N>
class PixelSampler;

template<typename T, typename ImageType, std::size_t N>
class ImageTexture : public Texture<T> {
public:
    ImageTexture(const Image<ImageType, N>& image, 
                 const TextureSamplingMode& mode);

    virtual void evaluate(const Vector3R& uvw, T* const out_value) const override = 0;

protected:
    Image<ImageType, N> _image;
    std::unique_ptr<PixelSampler<ImageType, N>> _pixelSampler;
};

} // namespace cadise

#include "core/texture/category/imageTexture.ipp"