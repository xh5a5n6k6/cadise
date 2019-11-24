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
    const Vector3R normal = surfaceIntersection.surfaceInfo().frontNormal();
    
    const real IdotN = surfaceIntersection.wi().dot(normal);
    const Spectrum reflectance = _fresnel.evaluateReflectance(IdotN);
    const real reflectionProbability = reflectance.average();
    const real sampleProbability = random::nextReal();
    
    bool isReflection = false;
    bool isRefraction = false;
    if (sampleProbability <= reflectionProbability) {
        isReflection = true;
    }
    else {
        isRefraction = true;
    }

    real etaI = _fresnel.iorOuter();
    real etaT = _fresnel.iorInner();
    Spectrum result(0.0_r);

    if (isReflection) {
        const real nFactor = (IdotN < 0.0_r) ? -1.0_r : 1.0_r;
        const Vector3R shadingNormal = surfaceIntersection.surfaceInfo().shadingNormal();
        const Vector3R reflectDirection = surfaceIntersection.wi().reflect(shadingNormal * nFactor);

        const real pdf = reflectionProbability;

        surfaceIntersection.setWo(reflectDirection);
        surfaceIntersection.setPdf(pdf);

        const real LdotN = reflectDirection.absDot(normal);

        const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
        Spectrum sampleSpectrum;
        _albedo->evaluate(uvw, &sampleSpectrum);

        result = sampleSpectrum * reflectance / LdotN;
    }
    else if (isRefraction) {
        const Vector3R refractDirection = surfaceIntersection.wi().refract(normal, etaI, etaT);
        if (refractDirection.isZero()) {
            return Spectrum(0.0_r);
        }

        const real cosThetaI = refractDirection.dot(normal);
        if (cosThetaI < 0.0_r) {
            math::swap(etaI, etaT);
        }

        const Spectrum transmittance = _fresnel.evaluateReflectance(cosThetaI).complement();

        const real btdfFactor = (etaT * etaT) / (etaI * etaI);
        const real pdf = 1.0_r - reflectionProbability;

        surfaceIntersection.setWo(refractDirection);
        surfaceIntersection.setPdf(pdf);

        const real LdotN = refractDirection.absDot(normal);

        const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
        Spectrum sampleSpectrum;
        _albedo->evaluate(uvw, &sampleSpectrum);

        result = sampleSpectrum * transmittance * btdfFactor / LdotN;
    }
    else {
        // something wrong
    }

    return result;
}

real PerfectDielectric::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise