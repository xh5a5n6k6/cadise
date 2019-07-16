#include "core/bsdf/category/specularTransmittion.h"

#include "core/surfaceIntersection.h"

#include <algorithm>

namespace cadise {

SpecularTransmittion::SpecularTransmittion() :
    SpecularTransmittion(Spectrum(0.0_r), 1.0_r, 1.5_r) {
}

SpecularTransmittion::SpecularTransmittion(const Spectrum& albedo, const real iorOuter, const real iorInner) :
    BSDF(BSDFType(BxDF_Type::SPECULAR_TRANSMITTION)),
    _albedo(albedo),
    _fresnel(iorOuter, iorInner) {
}

Spectrum SpecularTransmittion::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularTransmittion::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceGeometryInfo().normal();
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

    return _albedo * transmittance * btdfFactor / LdotN;
}

} // namespace cadise