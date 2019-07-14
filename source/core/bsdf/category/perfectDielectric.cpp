#include "core/bsdf/category/perfectDielectric.h"

#include "core/surfaceIntersection.h"

#include "math/random.h"

#include <algorithm>

namespace cadise {

PerfectDielectric::PerfectDielectric() :
    PerfectDielectric(Vector3R(), 1.0_r, 1.5_r) {
}

PerfectDielectric::PerfectDielectric(const Vector3R& albedo, const real iorOuter, const real iorInner) :
    BSDF(BSDFType(BxDF_Type::SPECULAR_REFLECTION) | BSDFType(BxDF_Type::SPECULAR_TRANSMITTION)),
    _albedo(albedo),
    _fresnel(iorOuter, iorInner) {
}

Vector3R PerfectDielectric::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Vector3R(0.0_r);
}

Vector3R PerfectDielectric::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceGeometryInfo().normal();
    real etaI = _fresnel.iorOuter();
    real etaT = _fresnel.iorInner();

    bool isReflection = false;
    bool isRefraction = false;
    real IdotN = surfaceIntersection.wi().dot(normal);
    real reflectionProbability = _fresnel.evaluateReflectance(IdotN);
    real sampleProbability = random::get1D();
    if (sampleProbability <= reflectionProbability) {
        isReflection = true;
    }
    else {
        isRefraction = true;
    }

    Vector3R result;
    if (isReflection) {
        real nFactor = (IdotN < 0.0_r) ? -1.0_r : 1.0_r;
        Vector3R reflectDirection = surfaceIntersection.wi().reflect(normal * nFactor);

        real reflectance = reflectionProbability;
        real pdf = reflectionProbability;

        surfaceIntersection.setWo(reflectDirection);
        surfaceIntersection.setPdf(pdf);

        real LdotN = reflectDirection.absDot(normal);

        result = _albedo * reflectance / LdotN;
    }
    else if (isRefraction) {
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
        real pdf = 1.0_r - reflectionProbability;

        surfaceIntersection.setWo(refractDirection);
        surfaceIntersection.setPdf(pdf);

        real LdotN = refractDirection.absDot(normal);

        result = _albedo * transmittance * btdfFactor / LdotN;
    }

    return result;
}

} // namespace cadise