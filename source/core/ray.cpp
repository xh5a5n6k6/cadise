#include "core/ray.h"

namespace cadise {

Ray::Ray() {
}

Ray::Ray(Vector3R origin, Vector3R direction, real minT, real maxT, int32 depth) : 
    _origin(origin), _direction(direction.normalize()), _minT(minT), _maxT(maxT), _depth(depth) {
}

Vector3R Ray::at(real t) {
    return _origin + _direction * t;
}

Vector3R Ray::origin() {
    return _origin;
}

Vector3R Ray::direction() {
    return _direction;
}

real Ray::minT() {
    return _minT;
}

real Ray::maxT() {
    return _maxT;
}

int32 Ray::depth() {
    return _depth;
}

void Ray::setMinT(real minT) {
    _minT = minT;
}

void Ray::setMaxT(real maxT) {
    _maxT = maxT;
}

} // namespace cadise