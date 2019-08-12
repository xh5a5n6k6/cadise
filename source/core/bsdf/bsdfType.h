#pragma once

#include "core/bsdf/bxdfType.h"

namespace cadise {

class BsdfType {
public:
    BsdfType();
    BsdfType(const BxdfType& type);
    BsdfType(const uint32 type);

    BsdfType operator|(const BsdfType& type);

private:
    uint32 _type;
};

} // namespace cadise