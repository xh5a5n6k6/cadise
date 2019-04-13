#pragma once

#include "core/material.h"

namespace cadise {

class BlinnPhong : public Material {
public:
	BlinnPhong(Vector3 albedo, float diffuseRatio, float exponent);

	//Vector3 _rgb;
	//float _ka, _kd, _ks;
	//float _shininess;
	//float _reflectance;
};

} // namespace cadise