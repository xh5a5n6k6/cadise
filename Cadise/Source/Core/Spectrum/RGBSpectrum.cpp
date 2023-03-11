#include "Core/Spectrum/RGBSpectrum.h"

#include "Foundation/Assertion.h"
#include "Math/Math.h"
#include "Math/TVector3.h"

namespace cadise
{

RGBSpectrum::RGBSpectrum() :
    TConceptualSpectrum<real, 3>() 
{}

RGBSpectrum::RGBSpectrum(const real value) :
    TConceptualSpectrum<real, 3>(value) 
{}

RGBSpectrum::RGBSpectrum(const Vector3R& value)
{
    _values[0] = value.x();
    _values[1] = value.y();
    _values[2] = value.z();
}

RGBSpectrum::RGBSpectrum(const TConceptualSpectrum<real, 3>& other) :
    TConceptualSpectrum<real, 3>(other) 
{}

RGBSpectrum::RGBSpectrum(const RGBSpectrum& other) = default;

void RGBSpectrum::setSrgb(const Vector3R& srgb)
{
    const Vector3R linearSrgb(
        math::inverse_gamma_correction(srgb.x()),
        math::inverse_gamma_correction(srgb.y()),
        math::inverse_gamma_correction(srgb.z()));

    this->setLinearSrgb(linearSrgb);
}

void RGBSpectrum::setLinearSrgb(const Vector3R& linearSrgb)
{
    _values[0] = linearSrgb.x();
    _values[1] = linearSrgb.y();
    _values[2] = linearSrgb.z();
}

void RGBSpectrum::transformToLinearSrgb(Vector3R* const out_linearSrgb) const
{
    CS_ASSERT(out_linearSrgb);

    out_linearSrgb->set(_values[0], _values[1], _values[2]);
}

real RGBSpectrum::luminance() const 
{
    return 0.0126_r * _values[0] +
           0.7152_r * _values[1] +
           0.0722_r * _values[2];
}

} // namespace cadise