#include "core/texture/mapper/sphericalMapper.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

#include <algorithm>
#include <cmath>

namespace cadise {

SphericalMapper::SphericalMapper() = default;

Vector3R SphericalMapper::mappingToUvw(const SurfaceInfo& surfaceInfo) const {
    //Vector3R hitPoint = intersection.surfaceGeometryInfo().point();
    //Vector3R vector = intersection.surfaceGeometryInfo().normal();
    const Vector3R vector = surfaceInfo.frontNormal();

    const real theta = std::acos(std::clamp(vector.y(), -1.0_r, 1.0_r));
    real phi = std::atan2(vector.x(), vector.z());
    if (phi < 0.0_r) {
        phi += 2.0_r * constant::PI;
    }

    return Vector3R(theta * constant::INV_PI, phi * constant::INV_TWO_PI, 0.0_r);
}

} // namespace cadise