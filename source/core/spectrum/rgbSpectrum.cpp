#include "core/spectrum/rgbSpectrum.h"

#include "fundamental/assertion.h"

namespace cadise {

RgbSpectrum::RgbSpectrum() :
    ConceptualSpectrum<3>() {
}

RgbSpectrum::RgbSpectrum(const real value) :
    ConceptualSpectrum<3>(value) {
}

RgbSpectrum::RgbSpectrum(const Vector3R& value) {
    _values[0] = value.x();
    _values[1] = value.y();
    _values[2] = value.z();
}

RgbSpectrum::RgbSpectrum(const ConceptualSpectrum<3>& other) :
    ConceptualSpectrum<3>(other) {
}

RgbSpectrum::RgbSpectrum(const RgbSpectrum& other) = default;

void RgbSpectrum::transformToRgb(Vector3R* const out_rgb) const {
    CADISE_ASSERT(out_rgb);

    *out_rgb = Vector3R(_values.x(), _values.y(), _values.z());
}

real RgbSpectrum::luminance() const {
    return 0.0126_r * _values[0] +
           0.7152_r * _values[1] +
           0.0722_r * _values[2];
}

} // namespace cadise