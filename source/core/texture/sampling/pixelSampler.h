#pragma once

#include "math/type/imageType.h"
#include "math/type/mathType.h"

namespace cadise {

template<typename T, std::size_t N>
class PixelSampler {
public:
    virtual void sample(
        const Vector3R& uvw, 
        const Image<T, N>& image, 
        Vector<T, N>* const out_value) const = 0;
};

} // namespace cadise