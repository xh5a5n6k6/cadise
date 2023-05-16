#pragma once

#include "Core/Image/TImage.h"
#include "Math/Type/MathType.h"

#include <memory>

// forward declaration
namespace cadise
{
    class UVWWrapper;

    enum class ETextureWrapMode;
}

namespace cadise
{

template<typename T, std::size_t N>
class TPixelSampler
{
public:
    TPixelSampler(
        const TImage<T, N>&    image,
        const ETextureWrapMode mode);

    virtual ~TPixelSampler();

    virtual void sample(
        const Vector3R&               uvw, 
        TArithmeticArray<T, N>* const out_value) const = 0;

protected:
    TImage<T, N>                _image;
    std::unique_ptr<UVWWrapper> _uvwWrapper;
};

} // namespace cadise

#include "Core/Texture/Sampler/TPixelSampler.ipp"