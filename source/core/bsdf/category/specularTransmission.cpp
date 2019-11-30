#include "core/bsdf/category/specularTransmission.h"

#include "core/bsdf/fresnel/dielectricFresnel.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"

#include "math/math.h"

namespace cadise {


SpecularTransmission::SpecularTransmission(const std::shared_ptr<Texture<Spectrum>>& albedo,
                                           const std::shared_ptr<DielectricFresnel>& fresnel) :
    Bsdf(BsdfType(BxdfType::SPECULAR_TRANSMISSION)),
    _albedo(albedo),
    _fresnel(fresnel) {
}

Spectrum SpecularTransmission::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularTransmission::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().frontNormal();
    
    real etaI = _fresnel->iorOuter();
    real etaT = _fresnel->iorInner();

    const Vector3R refractDirection = surfaceIntersection.wi().refract(normal, etaI, etaT);
    if (refractDirection.isZero()) {
        return Spectrum(0.0_r);
    }

    real cosThetaI = refractDirection.dot(normal);
    if (cosThetaI < 0.0_r) {
        math::swap(etaI, etaT);
    }

    Spectrum reflectance;
    _fresnel->evaluateReflectance(cosThetaI, &reflectance);

    const Spectrum transmittance = reflectance.complement();
    const real btdfFactor = (etaT * etaT) / (etaI * etaI);
    const real pdf = 1.0_r;

    surfaceIntersection.setWo(refractDirection);
    surfaceIntersection.setPdf(pdf);

    const real LdotN = refractDirection.absDot(normal);

    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * transmittance * btdfFactor / LdotN;
}

real SpecularTransmission::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise