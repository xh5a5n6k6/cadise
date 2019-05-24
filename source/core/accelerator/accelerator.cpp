#include "core/accelerator/accelerator.h"

#include "core/color.h"

namespace cadise {

RGBColor Accelerator::emittance(Vector3F direction) {
    return RGBColor(0.0f, 0.0f, 0.0f);
}

Vector3F Accelerator::evaluateBSDF(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3F(0.0f, 0.0f, 0.0f);
}

Vector3F Accelerator::evaluateSampleBSDF(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3F(0.0f, 0.0f, 0.0f);
}

} // namespace cadise