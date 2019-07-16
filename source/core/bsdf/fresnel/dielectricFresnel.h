#pragma once

#include "core/bsdf/fresnel/fresnel.h"

namespace cadise {

class DielectricFresnel : public Fresnel {
public:
    DielectricFresnel(const real iorOuter, const real iorInner);

    Spectrum evaluateReflectance(const real cosThetaI) const override;

    real iorOuter() const;
    real iorInner() const;

private:
    real _iorOuter;
    real _iorInner;
};

} // namespace cadise