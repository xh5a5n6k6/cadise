#include "core/color.h"

namespace cadise {

RGBColor::RGBColor() :
    RGBColor(0.0f, 0.0f, 0.0f) {
}

RGBColor::RGBColor(float r, float g, float b) : 
    _rgb(r, g, b) {
}

RGBColor::RGBColor(Vector3F rgb) : 
    _rgb(rgb) {
}

Vector3F& RGBColor::rgb() {
    return _rgb;
}

float RGBColor::r() {
    return _rgb.x();
}

float RGBColor::g() {
    return _rgb.y();
}

float RGBColor::b() {
    return _rgb.z();
}

} // namespace cadise