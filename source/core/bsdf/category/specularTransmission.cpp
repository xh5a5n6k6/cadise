#include "core/bsdf/category/specularTransmission.h"

#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"

#include "math/math.h"

namespace cadise {


SpecularTransmission::SpecularTransmission(const std::shared_ptr<Texture<Spectrum>>& albedo,
                                           const real iorOuter, const real iorInner) :
    Bsdf(BsdfType(BxdfType::SPECULAR_TRANSMISSION)),
    _albedo(albedo),
    _fresnel(iorOuter, iorInner) {
}

Spectrum SpecularTransmission::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularTransmission::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceInfo().frontNormal();
    real etaI = _fresnel.iorOuter();
    real etaT = _fresnel.iorInner();

    Vector3R refractDirection = surfaceIntersection.wi().refract(normal, etaI, etaT);
    if (refractDirection.isZero()) {
        return Spectrum(0.0_r);
    }

    real cosThetaI = refractDirection.dot(normal);
    Spectrum transmittance = _fresnel.evaluateReflectance(cosThetaI).complement();
    if (cosThetaI < 0.0_r) {
        math::swap(etaI, etaT);
    }
    real btdfFactor = (etaT * etaT) / (etaI * etaI);
    real pdf = 1.0_r;

    surfaceIntersection.setWo(refractDirection);
    surfaceIntersection.setPdf(pdf);

    real LdotN = refractDirection.absDot(normal);

    Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * transmittance * btdfFactor / LdotN;
}

real SpecularTransmission::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise