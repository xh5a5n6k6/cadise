#pragma once

#include "Core/Texture/TTexture.h"

#include <memory>

namespace cadise
{

template<typename T>
class TCheckerboardTexture : public TTexture<T>
{
public:
    TCheckerboardTexture(
        const real                          oddTextureNumber,
        const real                          evenTextureNumber,
        const std::shared_ptr<TTexture<T>>& oddTexture,
        const std::shared_ptr<TTexture<T>>& evenTexture);

    void evaluate(const Vector3R& uvw, T* const out_value) const override;

private:
    std::shared_ptr<TTexture<T>> _oddTexture;
    std::shared_ptr<TTexture<T>> _evenTexture;

    real _oddTextureSize;
    real _evenTextureSize;
};

} // namespace cadise

#include "Core/Texture/Category/TCheckerboardTexture.ipp"