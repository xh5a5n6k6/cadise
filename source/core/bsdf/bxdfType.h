#pragma once

#include "math/type.h"

namespace cadise {

enum class BxdfType : uint32 {
    NONE                  = 0,
    DIFFUSE_REFLECTION    = 1 << 0,
    SPECULAR_REFLECTION   = 1 << 1,
    GLOSSY_REFLECTION     = 1 << 2,
    DIFFUSE_TRANSMITTION  = 1 << 3,
    SPECULAR_TRANSMITTION = 1 << 4,
    GLOSSY_TRANSMITTION   = 1 << 5,
};

} // namespace cadise