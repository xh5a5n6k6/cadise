#pragma once

#include "math/algebra.h"

namespace cadise {

class Ray {
public:
	Ray();
	Ray(Vector3 origin, Vector3 direction, float minT, float maxT, int depth = 0);

	Vector3 at(float t);

	Vector3& origin();
	Vector3& direction();
	float& maxT();
	float& minT();
	int& depth();
	//float& time();

private:
	Vector3 _origin, _direction;
	float _minT, _maxT;
	int _depth;
	float _time;
};

} // namespace cadise