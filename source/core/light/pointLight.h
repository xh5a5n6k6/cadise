#pragma once

#include "core/light.h"

namespace cadise {

class PointLight : public Light {
public:
	PointLight(Vector3 position, Vector3 color);

	Vector3& position();
	Vector3& color();

	Vector3 _position;
	Vector3 _color;
};

} // namespace cadise