#pragma once

#include "Core/Spectrum/Spectrum.h"

namespace cadise 
{

class Fresnel 
{
public:
    virtual ~Fresnel();

    virtual void evaluateReflectance(
        const real      cosThetaI,
        Spectrum* const out_reflectance) const = 0;
};

} // namespace cadise