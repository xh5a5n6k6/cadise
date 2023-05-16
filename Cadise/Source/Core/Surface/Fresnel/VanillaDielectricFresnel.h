#pragma once

#include "Core/Surface/Fresnel/DielectricFresnel.h"

namespace cadise
{

class VanillaDielectricFresnel : public DielectricFresnel
{
public:
    VanillaDielectricFresnel(
        const real iorOuter,
        const real iorInner);

    void evaluateReflectance(
        const real      cosThetaI,
        Spectrum* const out_reflectance) const override;
};

} // namespace cadise