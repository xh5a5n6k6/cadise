#pragma once

#include "math/algebra.h"

namespace cadise {

class HitSurface {
public:
	HitSurface() {}

	Vector3 _hit;
	Vector3 _normal;

	Vector3 _rgb;
	Vector3 _phongAttribute;
	float _s, _ref;
};

} // namespace cadise