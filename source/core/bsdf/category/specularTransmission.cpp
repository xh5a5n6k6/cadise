#include "core/bsdf/category/specularTransmission.h"

#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"

namespace cadise {


SpecularTransmission::SpecularTransmission(const std::shared_ptr<Texture<Spectrum>>& albedo,
                                           const real iorOuter, const real iorInner) :
    BSDF(BSDFType(BxDF_Type::SPECULAR_TRANSMITTION)),
    _albedo(albedo),
    _fresnel(iorOuter, iorInner) {
}

Spectrum SpecularTransmission::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularTransmission::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    real etaI = _fresnel.iorOuter();
    real etaT = _fresnel.iorInner();

    Vector3R refractDirection = surfaceIntersection.wi().refract(normal, etaI, etaT);
    if (refractDirection.isZero()) {
        return Spectrum(0.0_r);
    }

    real cosThetaI = refractDirection.dot(normal);
    Spectrum transmittance = Spectrum(1.0_r) - _fresnel.evaluateReflectance(cosThetaI);
    if (cosThetaI < 0.0_r) {
        std::swap(etaI, etaT);
    }
    real btdfFactor = (etaT * etaT) / (etaI * etaI);
    real pdf = 1.0_r;

    surfaceIntersection.setWo(refractDirection);
    surfaceIntersection.setPdf(pdf);

    real LdotN = refractDirection.absDot(normal);

    Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    return _albedo->evaluate(uvw) * transmittance * btdfFactor / LdotN;
}

} // namespace cadise