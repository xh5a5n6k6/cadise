#include "core/bxdf/lambertianBrdf.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

LambertianBRDF::LambertianBRDF(Vector3F albedo) :
    _albedo(albedo) {
}

Vector3F LambertianBRDF::evaluate(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    return _albedo * CADISE_INV_PI;
}

Vector3F LambertianBRDF::evaluateSample(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3F(0.0f, 0.0f, 0.0f);
}

} // namespace cadise