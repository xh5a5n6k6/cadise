#include "core/spectrum/rgbSpectrum.h"

#include "fundamental/assertion.h"

namespace cadise {

RgbSpectrum::RgbSpectrum() :
    ConceptualSpectrum<3>() {
}

RgbSpectrum::RgbSpectrum(const real v) :
    ConceptualSpectrum<3>(v) {
}

RgbSpectrum::RgbSpectrum(const Vector3R& v) {
    _values[0] = v.x();
    _values[1] = v.y();
    _values[2] = v.z();
}

RgbSpectrum::RgbSpectrum(const ConceptualSpectrum<3>& cs) :
    ConceptualSpectrum<3>(cs) {
}

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