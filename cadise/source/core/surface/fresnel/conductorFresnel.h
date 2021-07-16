#pragma once

#include "core/surface/fresnel/fresnel.h"

namespace cadise {

class ConductorFresnel : public Fresnel {
public:
    ConductorFresnel(
        const real      iorOuter, 
        const Spectrum& eta, 
        const Spectrum& k);

    void evaluateReflectance(
        const real      cosThetaI,
        Spectrum* const out_reflectance) const override = 0;

private:
    real     _iorOuter;
    Spectrum _eta;
    Spectrum _k;
};

} // namespace cadise