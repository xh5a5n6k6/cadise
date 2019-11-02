#pragma once

#include "core/texture/category/constantTexture.h"

namespace cadise {

template<typename T>
inline ConstantTexture<T>::ConstantTexture(const T& value) :
    _value(value) {
}

template<typename T>
inline void ConstantTexture<T>::evaluate(const Vector3R& uvw, T* const out_value) const {
    *out_value = _value;
}

} // namespace cadise