#include "core/bxdf/specularBrdf.h"

#include "core/surfaceInfo.h"

#include <algorithm>

namespace cadise {

SpecularBRDF::SpecularBRDF(Vector3R albedo) :
    _albedo(albedo) {
}

Vector3R SpecularBRDF::evaluate(Vector3R inDirection, Vector3R outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

Vector3R SpecularBRDF::evaluateSample(Vector3R inDirection, Vector3R &outDirection, SurfaceInfo &surfaceInfo) {
    outDirection = inDirection.reflect(surfaceInfo.normal());
    real LdotN = std::max(outDirection.dot(surfaceInfo.normal()), 0.0_r);

    return _albedo / LdotN;
}

} // namespace cadise