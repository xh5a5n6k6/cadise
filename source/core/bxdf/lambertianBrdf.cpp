#include "core/bxdf/lambertianBrdf.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

LambertianBRDF::LambertianBRDF(Vector3 albedo) :
    _albedo(albedo) {
}

Vector3 LambertianBRDF::evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    return _albedo * CADISE_INV_PI;
}

Vector3 LambertianBRDF::evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace cadise