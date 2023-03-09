#include "math/warp/sampleWarp.h"

#include "fundamental/assertion.h"
#include "math/tVector2.h"

#include <cmath>

namespace cadise
{

void SampleWarp::uniformTriangleUv(
    const std::array<real, 2>& sample,
    Vector2R* const            out_uv) 
{
    CS_ASSERT(out_uv);

    const real sqrtSeedA = std::sqrt(sample[0]);

    out_uv->set(1.0_r - sqrtSeedA, sqrtSeedA * sample[1]);
}

} // namespace cadise