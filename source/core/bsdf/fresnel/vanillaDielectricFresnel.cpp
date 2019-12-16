#include "core/bsdf/fresnel/vanillaDielectricFresnel.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <algorithm>

namespace cadise {

VanillaDielectricFresnel::VanillaDielectricFresnel(const real iorOuter, const real iorInner) :
    DielectricFresnel(iorOuter, iorInner) {
}

void VanillaDielectricFresnel::evaluateReflectance(const real cosThetaI,
                                                   Spectrum* const out_reflectance) const {
    CADISE_ASSERT(out_reflectance);

    real etaI = iorOuter();
    real etaT = iorInner();
    real cosI = cosThetaI;
    if (cosI < 0.0_r) {
        math::swap(etaI, etaT);
        cosI *= -1.0_r;
    }

    const real etaRatio = etaI / etaT;
    const real sinT2    = etaRatio * etaRatio * (1.0_r - cosI * cosI);

    // handle TIR condition
    if (sinT2 >= 1.0_r) {
        *out_reflectance = Spectrum(1.0_r);

        return;
    }

    const real cosT = std::sqrt(1.0_r - sinT2);

    const real rParallel      = (etaT * cosI - etaI * cosT) / (etaT * cosI + etaI * cosT);
    const real rPerpendicular = (etaI * cosI - etaT * cosT) / (etaI * cosI + etaT * cosT);
    const real rTotal         = 0.5_r * (rParallel * rParallel + rPerpendicular * rPerpendicular);

    *out_reflectance = Spectrum(rTotal);
}

} // namespace cadise