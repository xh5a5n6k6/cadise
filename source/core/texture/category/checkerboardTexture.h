#pragma once

#include "core/texture/texture.h"

#include <memory>

namespace cadise {

template<typename T>
class CheckerboardTexture : public Texture<T> {
public:
    CheckerboardTexture(const real oddTextureNumber, const real evenTextureNumber, 
                        const std::shared_ptr<Texture<T>>& oddTexture,
                        const std::shared_ptr<Texture<T>>& evenTexture);

    T evaluate(const Vector3R& uvw) const override;

private:
    std::shared_ptr<Texture<T>> _oddTexture;
    std::shared_ptr<Texture<T>> _evenTexture;

    real _oddTextureSize;
    real _evenTextureSize;
};

} // namespace cadise

#include "core/texture/category/checkerboardTexture.ipp"