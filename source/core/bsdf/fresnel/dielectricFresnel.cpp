#include "core/bsdf/fresnel/dielectricFresnel.h"

#include <algorithm>
#include <cmath>

namespace cadise {

DielectricFresnel::DielectricFresnel(const real iorOuter, const real iorInner) :
    _iorOuter(iorOuter),
    _iorInner(iorInner) {
}

real DielectricFresnel::evaluateReflectance(const real cosThetaI) const {
    real etaI = _iorOuter;
    real etaT = _iorInner;
    real cosI = cosThetaI;
    if (cosI < 0.0_r) {
        std::swap(etaI, etaT);
        cosI *= -1.0_r;
    }

    real etaRatio = etaI / etaT;
    real sin2_T = etaRatio * etaRatio * (1.0_r - cosI * cosI);

    // handle TIR condition
    if (sin2_T >= 1.0_r) {
        return 1.0_r;
    }
    real cosT = std::sqrt(1.0_r - sin2_T);

    real rParallel      = (etaT * cosI - etaI * cosT) / (etaT * cosI + etaI * cosT);
    real rPerpendicular = (etaI * cosI - etaT * cosT) / (etaI * cosI + etaT * cosT);
    real rTotal = 0.5_r * (rParallel * rParallel + rPerpendicular * rPerpendicular);

    return rTotal;
}

real DielectricFresnel::iorOuter() const {
    return _iorOuter;
}

real DielectricFresnel::iorInner() const {
    return _iorInner;
}

} // namespace cadise