#pragma once

#include "core/spectrum/conceptualSpectrum.h"

#include "math/mathType.h"

namespace cadise {

class RgbSpectrum : public ConceptualSpectrum<3> {
public:
    RgbSpectrum();
    RgbSpectrum(const real v);
    RgbSpectrum(const Vector3R& v);
    RgbSpectrum(const ConceptualSpectrum<3>& cs);

    Vector3R transformToRgb() const;
};

} // namespace cadise