#include "math/warp/sampleWarp.h"

#include "fundamental/assertion.h"
#include "math/tVector.h"

#include <cmath>

namespace cadise {

void SampleWarp::uniformTriangleUv(
    const Vector2R& sample,
    Vector2R* const out_uv) {

    CADISE_ASSERT(out_uv);

    const real sqrtSeedA = std::sqrt(sample.x());

    *out_uv = Vector2R(1.0_r - sqrtSeedA, sqrtSeedA * sample.y());
}

} // namespace cadise