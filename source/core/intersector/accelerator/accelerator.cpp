#include "core/intersector/accelerator/accelerator.h"

#include "core/color.h"
#include "core/surfaceInfo.h"

namespace cadise {

AABB3R Accelerator::bound() const {
    return AABB3R();
}

RGBColor Accelerator::emittance(const Vector3R direction) const {
    return RGBColor(0.0_r, 0.0_r, 0.0_r);
}

Vector3R Accelerator::evaluateBSDF(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

Vector3R Accelerator::evaluateSampleBSDF(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

} // namespace cadise