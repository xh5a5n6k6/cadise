#include "core/bxdf/specularBrdf.h"

#include "core/surfaceInfo.h"

namespace cadise {

SpecularBRDF::SpecularBRDF(Vector3 albedo) :
    _albedo(albedo) {
}

Vector3 SpecularBRDF::evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 SpecularBRDF::evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    outDirection = inDirection.reflect(surfaceInfo.hitNormal());
    float cosTheta = std::max(Dot(outDirection, surfaceInfo.hitNormal()), 0.0f);

    return _albedo / cosTheta;
}

} // namespace cadise