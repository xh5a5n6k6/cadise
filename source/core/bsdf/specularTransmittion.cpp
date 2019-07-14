#include "core/bsdf/specularTransmittion.h"

#include "core/surfaceIntersection.h"

#include <algorithm>

namespace cadise {

SpecularTransmittion::SpecularTransmittion() :
    SpecularTransmittion(Vector3R(), 1.0_r, 1.5_r) {
}

SpecularTransmittion::SpecularTransmittion(const Vector3R& albedo, const real iorOuter, const real iorInner) :
    BSDF(BSDFType(BxDF_Type::SPECULAR_TRANSMITTION)),
    _albedo(albedo),
    _fresnel(iorOuter, iorInner) {
}

Vector3R SpecularTransmittion::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Vector3R(0.0_r);
}

Vector3R SpecularTransmittion::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceGeometryInfo().normal();
    real etaI = _fresnel.iorOuter();
    real etaT = _fresnel.iorInner();

    Vector3R refractDirection = surfaceIntersection.wi().refract(normal, etaI, etaT);
    if (refractDirection.isZero()) {
        return Vector3R(0.0_r);
    }

    real cosThetaI = refractDirection.dot(normal);
    real transmittance = 1.0_r - _fresnel.evaluateReflectance(cosThetaI);
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