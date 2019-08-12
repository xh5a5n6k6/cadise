#pragma once

#include "core/texture/category/constantTexture.h"

namespace cadise {

template<typename T>
inline ConstantTexture<T>::ConstantTexture(const T& value) :
    _value(value) {
}

template<typename T>
inline T ConstantTexture<T>::evaluate(const Vector3R& uvw) const {
    return _value;
}

} // namespace cadise