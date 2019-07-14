#pragma once

#include "math/type.h"

namespace cadise {

class Fresnel {
public:
    virtual real evaluateReflectance(const real cosThetaI) const = 0;
};

} // namespace cadise