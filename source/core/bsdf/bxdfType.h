#pragma once

#include "math/type/fundamentalType.h"

namespace cadise {

enum class BxdfType : uint32 {
    NONE                  = 0,
    DIFFUSE_REFLECTION    = 1 << 0,
    SPECULAR_REFLECTION   = 1 << 1,
    GLOSSY_REFLECTION     = 1 << 2,
    DIFFUSE_TRANSMISSION  = 1 << 3,
    SPECULAR_TRANSMISSION = 1 << 4,
    GLOSSY_TRANSMISSION   = 1 << 5,
};

} // namespace cadise