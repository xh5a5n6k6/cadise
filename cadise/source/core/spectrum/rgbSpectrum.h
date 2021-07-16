#pragma once

#include "core/spectrum/tConceptualSpectrum.h"

#include "math/type/mathType.h"

namespace cadise {

class RgbSpectrum : public TConceptualSpectrum<real, 3> {
public:
    RgbSpectrum();
    explicit RgbSpectrum(const real value);
    explicit RgbSpectrum(const Vector3R& value);
    RgbSpectrum(const TConceptualSpectrum<real, 3>& other);
    RgbSpectrum(const RgbSpectrum& other);

    using Parent = TConceptualSpectrum<real, 3>;
    using Parent::Parent;

    // TODO: refactor here
    void setSrgb(const Vector3R& srgb);
    void setLinearSrgb(const Vector3R& linearSrgb);

    void transformToLinearSrgb(Vector3R* const out_linearSrgb) const;

    // TODO: refactor here
    real luminance() const;
};

} // namespace cadise