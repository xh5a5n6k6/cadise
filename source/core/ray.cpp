#include "core/ray.h"

#include "fundamental/assertion.h"
#include "math/constant.h"

#include <limits>

namespace cadise {

Ray::Ray() :
    Ray(Vector3R(0.0_r), Vector3R(0.0_r, 0.0_r, -1.0_r)) {
}

Ray::Ray(const Vector3R& origin, const Vector3R& direction) :
    Ray(origin, direction, constant::RAY_EPSILON, std::numeric_limits<real>::max()) {
}

Ray::Ray(const Vector3R& origin, const Vector3R& direction, const real minT, const real maxT) : 
    _origin(origin), 
    _direction(direction), 
    _minT(minT),
    _maxT(maxT) {

    CADISE_ASSERT(!direction.isZero());

    _direction = direction.normalize();
}

Vector3R Ray::at(const real t) const {
    return _origin + _direction * t;
}

const Vector3R& Ray::origin() const {
    return _origin;
}

const Vector3R& Ray::direction() const {
    return _direction;
}

real Ray::minT() const {
    return _minT;
}

real Ray::maxT() const {
    return _maxT;
}

void Ray::setOrigin(const Vector3R& origin) {
    _origin = origin;
}

void Ray::setDirection(const Vector3R& direction) {
    _direction = direction;
}

void Ray::setMinT(const real minT) {
    _minT = minT;
}

void Ray::setMaxT(const real maxT) {
    _maxT = maxT;
}

} // namespace cadise