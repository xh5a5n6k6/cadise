#pragma once

#include "Core/Spectrum/TConceptualSpectrum.h"

#include "Math/Type/MathType.h"

namespace cadise 
{

class RGBSpectrum : public TConceptualSpectrum<real, 3>
{
public:
    RGBSpectrum();
    explicit RGBSpectrum(const real value);
    explicit RGBSpectrum(const Vector3R& value);
    RGBSpectrum(const TConceptualSpectrum<real, 3>& other);
    RGBSpectrum(const RGBSpectrum& other);

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