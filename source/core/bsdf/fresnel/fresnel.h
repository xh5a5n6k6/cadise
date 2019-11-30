#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class Fresnel {
public:
    virtual void evaluateReflectance(const real cosThetaI,
                                     Spectrum* const out_reflectance) const = 0;
};

} // namespace cadise