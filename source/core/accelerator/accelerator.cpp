#include "core/accelerator/accelerator.h"

#include "core/color.h"

namespace cadise {

RGBColor Accelerator::emittance(Vector3 direction) {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

Vector3 Accelerator::evaluateBSDF(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 Accelerator::evaluateSampleBSDF(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace cadise