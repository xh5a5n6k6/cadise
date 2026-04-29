#pragma once

#include "Core/Texture/Category/TConstantTexture.h"

#include "Foundation/Assertion.h"

namespace cadise
{

template<typename T>
inline TConstantTexture<T>::TConstantTexture(const T& value) :
    _value(value)
{}

template<typename T>
inline void TConstantTexture<T>::evaluate(const Vector3R& uvw, T* const out_value) const
{
    CS_ASSERT(out_value);

    *out_value = _value;
}

} // namespace cadise