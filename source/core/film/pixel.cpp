#include "core/film/pixel.h"

namespace cadise {

Pixel::Pixel() :
    _value(0.0_r),
    _totalWeight(0.0_r) {
}

void Pixel::addValue(const Vector3R& value) {
    _value += value;
}

void Pixel::addWeight(const real weight) {
    _totalWeight += weight;
}

real Pixel::r() const {
    return _value.x();
}

real Pixel::g() const {
    return _value.y();
}

real Pixel::b() const {
    return _value.z();
}

real Pixel::weight() const {
    return _totalWeight;
}

} // namespace cadise