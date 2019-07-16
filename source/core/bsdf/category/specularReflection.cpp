#include "core/bsdf/category/specularReflection.h"

#include "core/surfaceIntersection.h"

#include <algorithm>

namespace cadise {

SpecularReflection::SpecularReflection(const Spectrum& albedo) :
    BSDF(BSDFType(BxDF_Type::SPECULAR_REFLECTION)),
    _albedo(albedo) {
}

Spectrum SpecularReflection::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularReflection::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceGeometryInfo().normal();
    Vector3R outDirection = surfaceIntersection.wi().reflect(normal);
    real LdotN = outDirection.absDot(normal);
    real pdf = 1.0_r;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    return _albedo / LdotN;
}

} // namespace cadise