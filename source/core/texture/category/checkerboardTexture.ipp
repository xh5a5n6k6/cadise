#pragma once

#include "core/texture/category/checkerboardTexture.h"

#include <cmath>

namespace cadise {

template<typename T>
inline CheckerboardTexture<T>::CheckerboardTexture(const real oddTextureNumber, const real evenTextureNumber,
                                                   const std::shared_ptr<Texture<T>>& oddTexture, 
                                                   const std::shared_ptr<Texture<T>>& evenTexture) :
    _oddTexture(oddTexture), 
    _evenTexture(evenTexture) {

    _oddTextureSize  = 1.0_r / oddTextureNumber;
    _evenTextureSize = 1.0_r / evenTextureNumber;
}

template<typename T>
inline T CheckerboardTexture<T>::evaluate(const Vector3R& uvw) const {
    int32 uCast = static_cast<int32>(std::floor(uvw.x() / _oddTextureSize));
    int32 vCast = static_cast<int32>(std::floor(uvw.y() / _evenTextureSize));
    
    if ((uCast + vCast) % 2 == 0) {
        return _evenTexture->evaluate(uvw);
    }
    else {
        return _oddTexture->evaluate(uvw);
    }
}

} // namespace cadise