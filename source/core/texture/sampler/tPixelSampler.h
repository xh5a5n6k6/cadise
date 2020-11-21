#pragma once

#include "math/type/imageType.h"
#include "math/type/mathType.h"

#include <memory>

namespace cadise {

enum class ETextureWrapMode;
class UvwWrapper;

template<typename T, std::size_t N>
class TPixelSampler {
public:
    explicit TPixelSampler(const ETextureWrapMode& mode);

    virtual ~TPixelSampler();

    virtual void sample(
        const Vector3R&      uvw, 
        const TImage<T, N>&  image, 
        TVector<T, N>* const out_value) const = 0;

protected:
    std::unique_ptr<UvwWrapper> _uvwWrapper;
};

} // namespace cadise

#include "core/texture/sampler/tPixelSampler.ipp"