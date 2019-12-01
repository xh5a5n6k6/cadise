#include "core/bsdf/category/specularReflection.h"

#include "core/bsdf/fresnel/dielectricFresnel.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"

#include <cmath>

namespace cadise {

SpecularReflection::SpecularReflection(const std::shared_ptr<Texture<Spectrum>>& albedo,
                                       const std::shared_ptr<DielectricFresnel>& fresnel) :
    Bsdf(BsdfType(BxdfType::SPECULAR_REFLECTION)),
    _albedo(albedo),
    _fresnel(fresnel) {

    CADISE_ASSERT(albedo);
    CADISE_ASSERT(fresnel);
}

Spectrum SpecularReflection::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularReflection::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R outDirection = surfaceIntersection.wi().reflect(normal);
    const real LdotN = outDirection.dot(normal);
    const real pdf = 1.0_r;

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdf);

    Spectrum reflectance;
    _fresnel->evaluateReflectance(LdotN, &reflectance);

    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return reflectance * sampleSpectrum / std::abs(LdotN);
}

real SpecularReflection::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise