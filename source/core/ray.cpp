#include "core/ray.h"

namespace cadise {

Ray::Ray(const Ray& ray) :
    _origin(ray._origin),
    _direction(ray._direction),
    _minT(ray._minT),
    _maxT(ray._maxT),
    _depth(ray._depth) {
}

Ray::Ray(const Vector3R& origin, const Vector3R& direction, const real minT, const real maxT, const int32 depth) : 
    _origin(origin), 
    _direction(direction.normalize()), 
    _minT(minT),
    _maxT(maxT),
    _depth(depth) {
}

Vector3R Ray::at(const real t) const {
    return _origin + _direction * t;
}

Vector3R Ray::origin() const {
    return _origin;
}

Vector3R Ray::direction() const {
    return _direction;
}

real Ray::minT() const {
    return _minT;
}

real Ray::maxT() const {
    return _maxT;
}

int32 Ray::depth() const {
    return _depth;
}

void Ray::setMinT(const real minT) {
    _minT = minT;
}

void Ray::setMaxT(const real maxT) {
    _maxT = maxT;
}

} // namespace cadise