#include "core/texture/mapper/sphericalMapper.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

SphericalMapper::SphericalMapper() = default;

void SphericalMapper::mappingToUvw(const Vector3R& direction, Vector3R* const out_uvw) const {
    CADISE_ASSERT(out_uvw);

    const Vector3R unitVector = direction.normalize();

    const real theta = std::acos(math::clamp(unitVector.y(), -1.0_r, 1.0_r));
    real phi = std::atan2(unitVector.x(), unitVector.z());
    if (phi < 0.0_r) {
        phi += constant::TWO_PI;
    }

    *out_uvw =  Vector3R(phi * constant::INV_TWO_PI,
                         (constant::PI - theta) * constant::INV_PI,
                         0.0_r);
}

} // namespace cadise