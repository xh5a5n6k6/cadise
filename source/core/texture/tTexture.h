#pragma once

#include "math/type/mathType.h"

namespace cadise {

template<typename T>
class TTexture {
public:
    virtual ~TTexture();

    virtual void evaluate(const Vector3R& uvw, T* const out_value) const = 0;
};

// template header implementation

template<typename T>
inline TTexture<T>::~TTexture() = default;

} // namespace cadise