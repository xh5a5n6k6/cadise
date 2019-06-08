#include "core/accelerator/accelerator.h"

#include "core/color.h"

namespace cadise {

AABB3R Accelerator::bound() {
    return AABB3R();
}

RGBColor Accelerator::emittance(Vector3R direction) {
    return RGBColor(0.0_r, 0.0_r, 0.0_r);
}

Vector3R Accelerator::evaluateBSDF(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

Vector3R Accelerator::evaluateSampleBSDF(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

} // namespace cadise