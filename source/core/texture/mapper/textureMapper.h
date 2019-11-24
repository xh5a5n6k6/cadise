#pragma once

#include "math/type/mathType.h"

namespace cadise {

class TextureMapper {
public:
    virtual void mappingToUvw(const Vector3R& direction, Vector3R* const out_uvw) const = 0;
};

} // namespace cadise