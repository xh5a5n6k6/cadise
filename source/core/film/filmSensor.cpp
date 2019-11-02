#include "core/film/filmSensor.h"

#include "math/vector.h"

namespace cadise {

FilmSensor::FilmSensor() :
    _r(0.0_r),
    _g(0.0_r),
    _b(0.0_r),
    _weight(0.0_r) {
}

void FilmSensor::addValue(const Vector3R& value) {
    addValue(value.x(), value.y(), value.z());
}

void FilmSensor::addValue(const real r, const real g, const real b) {
    _r += r;
    _g += g;
    _b += b;
}

void FilmSensor::addWeight(const real weight) {
    _weight += weight;
}

real FilmSensor::r() const {
    return _r;
}

real FilmSensor::g() const {
    return _g;
}

real FilmSensor::b() const {
    return _b;
}

real FilmSensor::weight() const {
    return _weight;
}

} // namespace cadise