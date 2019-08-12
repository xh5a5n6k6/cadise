#pragma once

#include "core/texture/texture.h"

namespace cadise {

template<typename T>
class ConstantTexture : public Texture<T> {
public:
    ConstantTexture(const T& value);

    T evaluate(const Vector3R& uvw) const override;

private:
    T _value;
};

} // namespace cadise

#include "core/texture/category/constantTexture.ipp"