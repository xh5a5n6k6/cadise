#pragma once

#include "core/texture/tTexture.h"

#include "core/imaging/tImage.h"
#include "core/texture/eTextureSampleMode.h"
#include "core/texture/eTextureWrapMode.h"

#include <memory>

namespace cadise {

template<typename T, std::size_t N>
class TPixelSampler;

template<typename T, typename ImageType, std::size_t N>
class TImageTexture : public TTexture<T> {
public:
    TImageTexture(
        const TImage<ImageType, N>& image, 
        const ETextureSampleMode&   sampleMode,
        const ETextureWrapMode&     wrapMode);

    virtual void evaluate(const Vector3R& uvw, T* const out_value) const override = 0;

protected:
    TImage<ImageType, N> _image;
    std::unique_ptr<TPixelSampler<ImageType, N>> _pixelSampler;
};

} // namespace cadise

#include "core/texture/category/tImageTexture.ipp"