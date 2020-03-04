#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

enum class BxdfType : uint32 {
    ABSORB                = 1 << 0,
    DIFFUSE_REFLECTION    = 1 << 1,
    SPECULAR_REFLECTION   = 1 << 2,
    GLOSSY_REFLECTION     = 1 << 3,
    DIFFUSE_TRANSMISSION  = 1 << 4,
    SPECULAR_TRANSMISSION = 1 << 5,
    GLOSSY_TRANSMISSION   = 1 << 6,
};

} // namespace cadise