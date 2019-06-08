#pragma once

#include "core/material/material.h"

namespace cadise {

class Mirror : public Material {
public:
    Mirror(Vector3R albedo);
};

} // namespace cadise