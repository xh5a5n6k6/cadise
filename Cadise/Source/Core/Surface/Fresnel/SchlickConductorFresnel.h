#pragma once

#include "Core/Surface/Fresnel/ConductorFresnel.h"

namespace cadise
{

class SchlickConductorFresnel : public ConductorFresnel
{
public:
    SchlickConductorFresnel(
        const real      iorOuter,
        const Spectrum& eta,
        const Spectrum& k);
    explicit SchlickConductorFresnel(const Spectrum& f0);

    void evaluateReflectance(
        const real      cosThetaI,
        Spectrum* const out_reflectance) const override;

private:
    // f0 means 0-degree fresnel reflection, use for approximation
    Spectrum _f0;
};

} // namespace cadise