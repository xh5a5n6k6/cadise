#include "core/color.h"

namespace cadise {

RGBColor::RGBColor() :
    RGBColor(0.0_r, 0.0_r, 0.0_r) {
}

RGBColor::RGBColor(const real r, const real g, const real b) : 
    _rgb(r, g, b) {
}

RGBColor::RGBColor(const Vector3R& rgb) : 
    _rgb(rgb) {
}

Vector3R& RGBColor::rgb() {
    return _rgb;
}

real RGBColor::r() const {
    return _rgb.x();
}

real RGBColor::g() const {
    return _rgb.y();
}

real RGBColor::b() const {
    return _rgb.z();
}

} // namespace cadise