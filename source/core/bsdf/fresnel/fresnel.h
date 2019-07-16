#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class Fresnel {
public:
    virtual Spectrum evaluateReflectance(const real cosThetaI) const = 0;
};

} // namespace cadise