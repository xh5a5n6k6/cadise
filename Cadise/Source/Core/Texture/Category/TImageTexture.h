#pragma once

#include "Core/Texture/TTexture.h"

#include "Core/Texture/ETextureSampleMode.h"
#include "Core/Texture/ETextureWrapMode.h"
#include "Math/Type/ImageType.h"

#include <memory>

// forward declaration
namespace cadise
{
    template<typename T, std::size_t N>
    class TPixelSampler;
}

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

#include "Core/Texture/Category/TImageTexture.ipp"