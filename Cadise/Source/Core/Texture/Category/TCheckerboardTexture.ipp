#pragma once

#include "Core/Texture/Category/TCheckerboardTexture.h"

#include "Foundation/Assertion.h"
#include "Math/TVector3.h"

#include <cmath>

namespace cadise
{

template<typename T>
inline TCheckerboardTexture<T>::TCheckerboardTexture(
    const real                          oddTextureNumber,
    const real                          evenTextureNumber,
    const std::shared_ptr<TTexture<T>>& oddTexture,
    const std::shared_ptr<TTexture<T>>& evenTexture) :

    _oddTexture(oddTexture),
    _evenTexture(evenTexture)
{
    CS_ASSERT(oddTexture);
    CS_ASSERT(evenTexture);

    _oddTextureSize  = 1.0_r / oddTextureNumber;
    _evenTextureSize = 1.0_r / evenTextureNumber;
}

template<typename T>
inline void TCheckerboardTexture<T>::evaluate(const Vector3R& uvw, T* const out_value) const
{
    CS_ASSERT(out_value);

    const int32 uCast = static_cast<int32>(std::floor(uvw.x() / _oddTextureSize));
    const int32 vCast = static_cast<int32>(std::floor(uvw.y() / _evenTextureSize));

    if ((uCast + vCast) % 2 == 0)
    {
        _evenTexture->evaluate(uvw, out_value);
    }
    else
    {
        _oddTexture->evaluate(uvw, out_value);
    }
}

} // namespace cadise