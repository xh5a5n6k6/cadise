#include "core/bsdf/category/specularReflection.h"

#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"

namespace cadise {

SpecularReflection::SpecularReflection(const std::shared_ptr<Texture<Spectrum>>& albedo) :
    BSDF(BSDFType(BxDF_Type::SPECULAR_REFLECTION)),
    _albedo(albedo) {
}

Spectrum SpecularReflection::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularReflection::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    Vector3R outDirection = surfaceIntersection.wi().reflect(normal);
    real LdotN = outDirection.absDot(normal);
    real pdf = 1.0_r;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    return _albedo->evaluate(uvw) / LdotN;
}

} // namespace cadise