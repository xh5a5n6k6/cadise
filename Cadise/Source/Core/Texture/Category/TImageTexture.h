#pragma once

#include "core/texture/tTexture.h"

#include "core/texture/eTextureSampleMode.h"
#include "core/texture/eTextureWrapMode.h"
#include "math/type/imageType.h"

#include <memory>

namespace cadise { template<typename T, std::size_t N> class TPixelSampler; }

namespace cadise 
{

template<typename T, typename ImageType, std::size_t N>
class TImageTexture : public TTexture<T>
{
public:
    TImageTexture(
        const TImage<ImageType, N>& image, 
        const ETextureSampleMode    sampleMode,
        const ETextureWrapMode      wrapMode);

    void evaluate(const Vector3R& uvw, T* const out_value) const override = 0;

protected:
    std::unique_ptr<TPixelSampler<ImageType, N>> _pixelSampler;
};

} // namespace cadise

#include "core/texture/category/tImageTexture.ipp"