#include "core/bsdf/category/perfectDielectric.h"

#include "core/surfaceIntersection.h"

#include "math/random.h"

#include <algorithm>

namespace cadise {

PerfectDielectric::PerfectDielectric() :
    PerfectDielectric(Spectrum(0.0_r), 1.0_r, 1.5_r) {
}

PerfectDielectric::PerfectDielectric(const Spectrum& albedo, const real iorOuter, const real iorInner) :
    BSDF(BSDFType(BxDF_Type::SPECULAR_REFLECTION) | BSDFType(BxDF_Type::SPECULAR_TRANSMITTION)),
    _albedo(albedo),
    _fresnel(iorOuter, iorInner) {
}

Spectrum PerfectDielectric::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum PerfectDielectric::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceGeometryInfo().normal();
    real etaI = _fresnel.iorOuter();
    real etaT = _fresnel.iorInner();

    bool isReflection = false;
    bool isRefraction = false;
    real IdotN = surfaceIntersection.wi().dot(normal);
    Spectrum reflectance = _fresnel.evaluateReflectance(IdotN);
    real reflectionProbability = reflectance.average();
    real sampleProbability = random::get1D();
    if (sampleProbability <= reflectionProbability) {
        isReflection = true;
    }
    else {
        isRefraction = true;
    }

    Spectrum result(0.0_r);
    if (isReflection) {
        real nFactor = (IdotN < 0.0_r) ? -1.0_r : 1.0_r;
        Vector3R reflectDirection = surfaceIntersection.wi().reflect(normal * nFactor);

        real pdf = reflectionProbability;

        surfaceIntersection.setWo(reflectDirection);
        surfaceIntersection.setPdf(pdf);

        real LdotN = reflectDirection.absDot(normal);

        result = _albedo * reflectance / LdotN;
    }
    else if (isRefraction) {
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
        real pdf = 1.0_r - reflectionProbability;

        surfaceIntersection.setWo(refractDirection);
        surfaceIntersection.setPdf(pdf);

        real LdotN = refractDirection.absDot(normal);

        result = _albedo * transmittance * btdfFactor / LdotN;
    }

    return result;
}

} // namespace cadise