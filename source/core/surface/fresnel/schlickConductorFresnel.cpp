#include "core/surface/fresnel/schlickConductorFresnel.h"

#include "fundamental/assertion.h"

#include <cmath>

namespace cadise {

SchlickConductorFresnel::SchlickConductorFresnel(
    const real iorOuter,
    const Spectrum& eta,
    const Spectrum& k) :
    
    ConductorFresnel(iorOuter, eta, k) {

    const Spectrum k2          = k.square();
    const Spectrum numerator   = (eta - Spectrum(iorOuter)).square() + k2;
    const Spectrum denominator = (eta + Spectrum(iorOuter)).square() + k2;

    _f0 = numerator / denominator;
}

SchlickConductorFresnel::SchlickConductorFresnel(const Spectrum& f0) :
    ConductorFresnel(1.0_r, Spectrum(1.0_r), Spectrum(1.0_r)),
    _f0(f0) {
}

void SchlickConductorFresnel::evaluateReflectance(
    const real      cosThetaI,
    Spectrum* const out_reflectance) const {
    
    CADISE_ASSERT(out_reflectance);

    const real     absCosThetaI   = std::abs(cosThetaI);
    const real     cosIComplement = 1.0_r - absCosThetaI;
    const Spectrum f0Complement   = _f0.complement();

    *out_reflectance = _f0 + f0Complement * cosIComplement 
                                          * (cosIComplement * cosIComplement) 
                                          * (cosIComplement * cosIComplement);
}

} // namespace cadise