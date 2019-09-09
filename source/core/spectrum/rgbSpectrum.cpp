#include "core/spectrum/rgbSpectrum.h"

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

Vector3R RgbSpectrum::transformToRgb() const {
    return Vector3R(_values.x(), _values.y(), _values.z());
}

} // namespace cadise