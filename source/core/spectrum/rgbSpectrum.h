#pragma once

#include "core/spectrum/conceptualSpectrum.h"

#include "math/type/mathType.h"

namespace cadise {

class RgbSpectrum : public ConceptualSpectrum<3> {
public:
    RgbSpectrum();
    RgbSpectrum(const real v);
    RgbSpectrum(const Vector3R& v);
    RgbSpectrum(const ConceptualSpectrum<3>& cs);

    void transformToRgb(Vector3R* const out_rgb) const;
};

} // namespace cadise