#include "core/ray.h"

namespace cadise {

Ray::Ray() {
}

Ray::Ray(Vector3F origin, Vector3F direction, float minT, float maxT, int depth) : 
    _origin(origin), _direction(direction.normalize()), _minT(minT), _maxT(maxT), _depth(depth) {
}

Vector3F Ray::at(float t) {
    return _origin + _direction * t;
}

Vector3F Ray::origin() {
    return _origin;
}

Vector3F Ray::direction() {
    return _direction;
}

float Ray::minT() {
    return _minT;
}

float Ray::maxT() {
    return _maxT;
}

int Ray::depth() {
    return _depth;
}

float Ray::time() {
    return _time;
}

void Ray::setMinT(float minT) {
    _minT = minT;
}

void Ray::setMaxT(float maxT) {
    _maxT = maxT;
}

} // namespace cadise