#pragma once

#include "core/material/material.h"

namespace cadise {

class Mirror : public Material {
public:
    Mirror(const Vector3R albedo);
};

} // namespace cadise