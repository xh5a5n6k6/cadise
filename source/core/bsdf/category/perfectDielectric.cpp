#include "core/bsdf/category/perfectDielectric.h"

#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"

#include "math/math.h"
#include "math/random.h"

namespace cadise {

PerfectDielectric::PerfectDielectric(const std::shared_ptr<Texture<Spectrum>>& albedo, 
                                     const real iorOuter, const real iorInner) :
    Bsdf(BsdfType(BxdfType::SPECULAR_REFLECTION) | BsdfType(BxdfType::SPECULAR_TRANSMISSION)),
    _albedo(albedo),
    _fresnel(iorOuter, iorInner) {
}

Spectrum PerfectDielectric::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum PerfectDielectric::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceInfo().frontNormal();
    real etaI = _fresnel.iorOuter();
    real etaT = _fresnel.iorInner();

    bool isReflection = false;
    bool isRefraction = false;
    real IdotN = surfaceIntersection.wi().dot(normal);
    Spectrum reflectance = _fresnel.evaluateReflectance(IdotN);
    real reflectionProbability = reflectance.average();
    real sampleProbability = random::nextReal();
    if (sampleProbability <= reflectionProbability) {
        isReflection = true;
    }
    else {
        isRefraction = true;
    }

    Spectrum result(0.0_r);
    if (isReflection) {
        real nFactor = (IdotN < 0.0_r) ? -1.0_r : 1.0_r;
        Vector3R shadingNormal = surfaceIntersection.surfaceInfo().shadingNormal();
        Vector3R reflectDirection = surfaceIntersection.wi().reflect(shadingNormal * nFactor);

        real pdf = reflectionProbability;

        surfaceIntersection.setWo(reflectDirection);
        surfaceIntersection.setPdf(pdf);

        real LdotN = reflectDirection.absDot(normal);

        Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
        result = _albedo->evaluate(uvw) * reflectance / LdotN;
    }
    else if (isRefraction) {
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
        real pdf = 1.0_r - reflectionProbability;

        surfaceIntersection.setWo(refractDirection);
        surfaceIntersection.setPdf(pdf);

        real LdotN = refractDirection.absDot(normal);

        Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
        result = _albedo->evaluate(uvw) * transmittance * btdfFactor / LdotN;
    }

    return result;
}

real PerfectDielectric::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise