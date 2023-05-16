#pragma once

#include "Math/Type/PrimitiveType.h"

namespace cadise
{

/*! @brief BSDF (BRDF + BTDF) lobe type.
*/
enum class ELobe : uint32
{
    Undefined = 0,

    Absorb               = 1 << 0,
    DiffuseReflection    = 1 << 1,
    SpecularReflection   = 1 << 2,
    GlossyReflection     = 1 << 3,
    DiffuseTransmission  = 1 << 4,
    SpecularTransmission = 1 << 5,
    GlossyTransmission   = 1 << 6,
};

} // namespace cadise