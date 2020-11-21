#pragma once

#include "core/spectrum/tConceptualSpectrum.h"

#include "math/type/mathType.h"

namespace cadise {

class RgbSpectrum : public TConceptualSpectrum<3> {
public:
    RgbSpectrum();
    explicit RgbSpectrum(const real value);
    explicit RgbSpectrum(const Vector3R& value);
    RgbSpectrum(const TConceptualSpectrum<3>& other);
    RgbSpectrum(const RgbSpectrum& other);

    using Parent = TConceptualSpectrum<3>;
    using Parent::Parent;

    void transformToRgb(Vector3R* const out_rgb) const;

    // TODO: refactor here
    real luminance() const;
};

} // namespace cadise