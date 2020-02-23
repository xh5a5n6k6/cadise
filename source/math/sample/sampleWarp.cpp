#include "math/sample/sampleWarp.h"

#include "fundamental/assertion.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

void SampleWarp::uniformTriangle(
    const Vector2R& sample,
    Vector2R* const out_uv) {

    CADISE_ASSERT(out_uv);

    const real sqrtSeed1 = std::sqrt(sample.x());

    *out_uv = Vector2R(1.0_r - sqrtSeed1, sqrtSeed1 * sample.y());
}

} // namespace cadise