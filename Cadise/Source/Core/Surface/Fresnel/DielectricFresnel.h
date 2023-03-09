#pragma once

#include "core/surface/fresnel/fresnel.h"

namespace cadise 
{

class DielectricFresnel : public Fresnel 
{
public:
    DielectricFresnel(const real iorOuter, const real iorInner);

    void evaluateReflectance(
        const real      cosThetaI,
        Spectrum* const out_reflectance) const override = 0;

    real iorOuter() const;
    real iorInner() const;

private:
    real _iorOuter;
    real _iorInner;
};

} // namespace cadise