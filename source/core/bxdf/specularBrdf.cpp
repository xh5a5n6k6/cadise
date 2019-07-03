#include "core/bxdf/specularBrdf.h"

#include "core/surfaceInfo.h"

#include <algorithm>

namespace cadise {

SpecularBRDF::SpecularBRDF(const Vector3R albedo) :
    _albedo(albedo) {
}

Vector3R SpecularBRDF::evaluate(const Vector3R inDirection, const Vector3R outDirection, const SurfaceInfo surfaceInfo) const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

Vector3R SpecularBRDF::evaluateSample(const Vector3R inDirection, Vector3R &outDirection, const SurfaceInfo surfaceInfo, real &pdf) const {
    outDirection = inDirection.reflect(surfaceInfo.normal());
    real LdotN = std::max(outDirection.dot(surfaceInfo.normal()), 0.0_r);
    pdf = 1.0_r;

    return _albedo / LdotN;
}

} // namespace cadise