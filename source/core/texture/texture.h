#pragma once

#include "math/type/mathType.h"

namespace cadise {

template<typename T>
class Texture {
public:
    virtual ~Texture();

    virtual void evaluate(const Vector3R& uvw, T* const out_value) const = 0;
};

// template header implementation

template<typename T>
inline Texture<T>::~Texture() = default;

} // namespace cadise