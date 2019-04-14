#pragma once

#include "core/material/material.h"

namespace cadise {

class BlinnPhong : public Material {
public:
    BlinnPhong(Vector3 albedo, float diffuseRatio, float exponent);

};

} // namespace cadise