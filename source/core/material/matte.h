#pragma once

#include "core/material/material.h"

namespace cadise {

class Matte : public Material {
public:
    Matte(Vector3 albedo);
};

} // namespace cadise