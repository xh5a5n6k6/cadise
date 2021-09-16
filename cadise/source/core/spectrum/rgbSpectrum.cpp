#include "core/spectrum/rgbSpectrum.h"

#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/tVector3.h"

namespace cadise
{

RgbSpectrum::RgbSpectrum() :
    TConceptualSpectrum<real, 3>() 
{}

RgbSpectrum::RgbSpectrum(const real value) :
    TConceptualSpectrum<real, 3>(value) 
{}

RgbSpectrum::RgbSpectrum(const Vector3R& value)
{
    _values[0] = value.x();
    _values[1] = value.y();
    _values[2] = value.z();
}

RgbSpectrum::RgbSpectrum(const TConceptualSpectrum<real, 3>& other) :
    TConceptualSpectrum<real, 3>(other) 
{}

RgbSpectrum::RgbSpectrum(const RgbSpectrum& other) = default;

void RgbSpectrum::setSrgb(const Vector3R& srgb)
{
    const Vector3R linearSrgb(
        math::inverse_gamma_correction(srgb.x()),
        math::inverse_gamma_correction(srgb.y()),
        math::inverse_gamma_correction(srgb.z()));

    this->setLinearSrgb(linearSrgb);
}

void RgbSpectrum::setLinearSrgb(const Vector3R& linearSrgb)
{
    _values[0] = linearSrgb.x();
    _values[1] = linearSrgb.y();
    _values[2] = linearSrgb.z();
}

void RgbSpectrum::transformToLinearSrgb(Vector3R* const out_linearSrgb) const
{
    CS_ASSERT(out_linearSrgb);

    out_linearSrgb->set(_values[0], _values[1], _values[2]);
}

real RgbSpectrum::luminance() const 
{
    return 0.0126_r * _values[0] +
           0.7152_r * _values[1] +
           0.0722_r * _values[2];
}

} // namespace cadise