#include "core/bxdf/lambertianBrdf.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

LambertianBRDF::LambertianBRDF(const Vector3R albedo) :
    _albedo(albedo) {
}

Vector3R LambertianBRDF::evaluate(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    return _albedo * constant::INV_PI;
}

Vector3R LambertianBRDF::evaluateSample(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo) const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

} // namespace cadise