#pragma once

#include "core/imaging/tImage.h"
#include "math/type/mathType.h"

#include <memory>

namespace cadise {

class UvwWrapper;
enum class ETextureWrapMode;

template<typename T, std::size_t N>
class TPixelSampler {
public:
    TPixelSampler(
        const TImage<T, N>&     image,
        const ETextureWrapMode& mode);

    virtual ~TPixelSampler();

    virtual void sample(
        const Vector3R&               uvw, 
        TArithmeticArray<T, N>* const out_value) const = 0;

protected:
    TImage<T, N>                _image;
    std::unique_ptr<UvwWrapper> _uvwWrapper;
};

} // namespace cadise

#include "core/texture/sampler/tPixelSampler.ipp"