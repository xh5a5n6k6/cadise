#include "core/ray.h"

namespace cadise {

Ray::Ray() {
}

Ray::Ray(Vector3 origin, Vector3 direction, float minT, float maxT, int depth) : 
	_origin(origin), _direction(direction.normalize()), _minT(minT), _maxT(maxT), _depth(depth) {
}

Vector3 Ray::at(float t) {
	return _origin + _direction * t;
}

Vector3& Ray::origin() {
	return _origin;
}

Vector3& Ray::direction() {
	return _direction;
}

float& Ray::maxT() {
	return _maxT;
}

float& Ray::minT() {
	return _minT;
}

int& Ray::depth() {
	return _depth;
}

} // namespace cadise