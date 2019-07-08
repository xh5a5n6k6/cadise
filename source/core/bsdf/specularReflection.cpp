#include "core/bsdf/specularReflection.h"

#include "core/surfaceIntersection.h"

#include <algorithm>

namespace cadise {

SpecularReflection::SpecularReflection(const Vector3R& albedo) :
    _albedo(albedo) {
}

Vector3R SpecularReflection::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Vector3R(0.0_r);
}

Vector3R SpecularReflection::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R outDirection = surfaceIntersection.wi().reflect(surfaceIntersection.surfaceGeometryInfo().normal());
    real LdotN = std::max(outDirection.dot(surfaceIntersection.surfaceGeometryInfo().normal()), 0.0_r);
    real pdf = 1.0_r;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    return _albedo / LdotN;
}

} // namespace cadise