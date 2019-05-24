#pragma once

#include "core/material/material.h"

namespace cadise {

class Matte : public Material {
public:
    Matte(Vector3F albedo);
};

} // namespace cadise