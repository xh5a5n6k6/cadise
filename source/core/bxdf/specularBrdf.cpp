#include "core/bxdf/specularBrdf.h"

#include "core/surfaceInfo.h"

namespace cadise {

Vector3 SpecularBRDF::evaluate(Vector3 inDirection, Vector3 outDirection, SurfaceInfo &surfaceInfo) {
    return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 SpecularBRDF::evaluateSample(Vector3 inDirection, Vector3 &outDirection, SurfaceInfo &surfaceInfo) {
    outDirection = 2.0f * surfaceInfo.hitNormal() - inDirection;

    return Vector3(1.0f, 1.0f, 1.0f);
}

} // namespace cadise