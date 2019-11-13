#pragma once

#include "math/type/mathType.h"

namespace cadise {

template<typename T>
class Texture {
public:
    virtual void evaluate(const Vector3R& uvw, T* const out_value) const = 0;
};

} // namespace cadise