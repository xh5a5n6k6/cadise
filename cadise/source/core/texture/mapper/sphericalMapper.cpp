#include "core/texture/mapper/sphericalMapper.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/tVector3.h"

namespace cadise {

SphericalMapper::SphericalMapper() = default;

void SphericalMapper::mappingToUvw(const Vector3R& direction, Vector3R* const out_uvw) const {
    CADISE_ASSERT(out_uvw);

    const Vector3R unitVector = direction.normalize();

    const real theta  = std::acos(math::clamp(unitVector.y(), -1.0_r, 1.0_r));
    const real phiRaw = std::atan2(unitVector.x(), unitVector.z());
    const real phi    = (phiRaw < 0.0_r) ? phiRaw + constant::two_pi<real> : phiRaw;

    out_uvw->set(
        phi * constant::rcp_two_pi<real>,
        (constant::pi<real> - theta) * constant::rcp_pi<real>,
        0.0_r);
}

} // namespace cadise