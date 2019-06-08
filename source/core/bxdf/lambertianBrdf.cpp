#include "core/bxdf/lambertianBrdf.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

LambertianBRDF::LambertianBRDF(Vector3R albedo) :
    _albedo(albedo) {
}

Vector3R LambertianBRDF::evaluate(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) {
    return _albedo * constant::INV_PI;
}

Vector3R LambertianBRDF::evaluateSample(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

} // namespace cadise