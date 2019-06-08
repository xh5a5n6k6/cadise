#include "core/color.h"

namespace cadise {

RGBColor::RGBColor() :
    RGBColor(0.0_r, 0.0_r, 0.0_r) {
}

RGBColor::RGBColor(real r, real g, real b) : 
    _rgb(r, g, b) {
}

RGBColor::RGBColor(Vector3R rgb) : 
    _rgb(rgb) {
}

Vector3R& RGBColor::rgb() {
    return _rgb;
}

real RGBColor::r() {
    return _rgb.x();
}

real RGBColor::g() {
    return _rgb.y();
}

real RGBColor::b() {
    return _rgb.z();
}

} // namespace cadise