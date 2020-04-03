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
        phi += constant::two_pi<real>;
    }

    *out_uvw =  Vector3R(phi * constant::inv_two_pi<real>,
                         (constant::pi<real> - theta) * constant::inv_pi<real>,
                         0.0_r);
}

} // namespace cadise