#include "core/film/rgbRadianceSensor.h"

#include "math/tVector.h"

namespace cadise {

RgbRadianceSensor::RgbRadianceSensor() :
    _r(0.0_r),
    _g(0.0_r),
    _b(0.0_r),
    _weight(0.0_r) {
}

void RgbRadianceSensor::addValue(const Vector3R& value) {
    this->addValue(value.x(), value.y(), value.z());
}

void RgbRadianceSensor::addValue(const real r, const real g, const real b) {
    _r += r;
    _g += g;
    _b += b;
}

void RgbRadianceSensor::addWeight(const real weight) {
    _weight += weight;
}

real RgbRadianceSensor::r() const {
    return _r;
}

real RgbRadianceSensor::g() const {
    return _g;
}

real RgbRadianceSensor::b() const {
    return _b;
}

real RgbRadianceSensor::weight() const {
    return _weight;
}

} // namespace cadise