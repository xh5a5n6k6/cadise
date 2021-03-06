#pragma once

#include "math/type/mathType.h"

namespace cadise {

class SampleWarp {
public:
    static void uniformTriangleUv(
        const Vector2R& sample,
        Vector2R* const out_uv);
};

} // namespace cadise