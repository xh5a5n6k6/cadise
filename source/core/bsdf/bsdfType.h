#pragma once

#include "core/bsdf/bxdfType.h"

namespace cadise {

class BSDFType {
public:
    BSDFType();
    BSDFType(const BxDF_Type& type);
    BSDFType(const uint32 type);

    BSDFType operator|(const BSDFType& type);

private:
    uint32 _type;
};

} // namespace cadise