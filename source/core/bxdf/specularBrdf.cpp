#include "core/bxdf/specularBrdf.h"

#include "core/surfaceInfo.h"

#include <algorithm>

namespace cadise {

SpecularBRDF::SpecularBRDF(Vector3F albedo) :
    _albedo(albedo) {
}

Vector3F SpecularBRDF::evaluate(Vector3F inDirection, Vector3F outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3F(0.0f, 0.0f, 0.0f);
}

Vector3F SpecularBRDF::evaluateSample(Vector3F inDirection, Vector3F &outDirection, SurfaceInfo &surfaceInfo) {
    outDirection = inDirection.reflect(surfaceInfo.normal());
    float cosTheta = std::max(outDirection.dot(surfaceInfo.normal()), 0.0f);

    return _albedo / cosTheta;
}

} // namespace cadise