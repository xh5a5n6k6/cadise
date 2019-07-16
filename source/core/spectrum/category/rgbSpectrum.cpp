#include "core/spectrum/category/rgbSpectrum.h"

namespace cadise {

RGBSpectrum::RGBSpectrum() :
    ConceptualSpectrum<3>() {
}

RGBSpectrum::RGBSpectrum(const real v) :
    ConceptualSpectrum<3>(v) {
}

RGBSpectrum::RGBSpectrum(const Vector3R& v) {
    _values[0] = v.x();
    _values[1] = v.y();
    _values[2] = v.z();
}

RGBSpectrum::RGBSpectrum(const ConceptualSpectrum<3>& cs) :
    ConceptualSpectrum<3>(cs) {
}

void RGBSpectrum::transformIntoRGB(Vector3R& rgb) const {
    rgb = Vector3R(_values.x(), _values.y(), _values.z());
}

} // namespace cadise