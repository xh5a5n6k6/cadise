#pragma once

#include "math/mathType.h"

namespace cadise {

template<typename T>
class Texture {
public:
    virtual T evaluate(const Vector3R& uvw) const = 0;
};

} // namespace cadise

#include "core/texture/texture.ipp"