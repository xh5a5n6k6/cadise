#pragma once

#include "Core/Texture/TTexture.h"

namespace cadise
{

template<typename T>
class TConstantTexture : public TTexture<T>
{
public:
    explicit TConstantTexture(const T& value);

    void evaluate(const Vector3R& uvw, T* const out_value) const override;

private:
    T _value;
};

} // namespace cadise

#include "Core/Texture/Category/TConstantTexture.ipp"