#include "core/ray.h"

#include "math/constant.h"

#include <limits>

namespace cadise {

Ray::Ray(const Vector3R& origin, const Vector3R& direction) :
    Ray(origin, direction, constant::RAY_EPSILON, std::numeric_limits<real>::max()) {
}

Ray::Ray(const Vector3R& origin, const Vector3R& direction, const real minT, const real maxT) : 
    _origin(origin), 
    _direction(direction.normalize()), 
    _minT(minT),
    _maxT(maxT) {
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

void Ray::setMinT(const real minT) {
    _minT = minT;
}

void Ray::setMaxT(const real maxT) {
    _maxT = maxT;
}

} // namespace cadise