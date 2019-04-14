#pragma once

#include "math/algebra.h"

namespace cadise {

class Light {
public:
    virtual Vector3& position() = 0;
    virtual Vector3& color() = 0;
};

} // namespace cadise