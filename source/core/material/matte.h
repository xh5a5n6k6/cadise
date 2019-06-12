#pragma once

#include "core/material/material.h"

namespace cadise {

class Matte : public Material {
public:
    Matte(const Vector3R albedo);
};

} // namespace cadise