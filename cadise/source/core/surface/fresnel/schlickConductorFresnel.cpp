#include "core/surface/fresnel/schlickConductorFresnel.h"

#include "fundamental/assertion.h"

#include <cmath>

namespace cadise {

SchlickConductorFresnel::SchlickConductorFresnel(
    const real iorOuter,
    const Spectrum& eta,
    const Spectrum& k) :
    
    ConductorFresnel(iorOuter, eta, k) {

    const Spectrum k2          = k.squared();
    const Spectrum numerator   = eta.sub(iorOuter).squared().add(k2);
    const Spectrum denominator = eta.add(iorOuter).squared().add(k2);

    _f0 = numerator.div(denominator);
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

    const Spectrum addFactor = f0Complement.mul(
        cosIComplement *
        (cosIComplement * cosIComplement) *
        (cosIComplement * cosIComplement));

    out_reflectance->set(_f0.add(addFactor));
}

} // namespace cadise