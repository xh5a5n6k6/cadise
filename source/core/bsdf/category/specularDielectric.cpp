#include "core/bsdf/category/specularDielectric.h"

#include "core/bsdf/fresnel/dielectricFresnel.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/random.h"

namespace cadise {

SpecularDielectric::SpecularDielectric(const std::shared_ptr<Texture<Spectrum>>& albedo, 
                                       const std::shared_ptr<DielectricFresnel>& fresnel) :
    Bsdf(BsdfType(BxdfType::SPECULAR_REFLECTION) | BsdfType(BxdfType::SPECULAR_TRANSMISSION)),
    _albedo(albedo),
    _fresnel(fresnel) {

    CADISE_ASSERT(albedo);
    CADISE_ASSERT(fresnel);
}

Spectrum SpecularDielectric::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

Spectrum SpecularDielectric::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();
    
    const real VdotN = V.dot(Ns);
    
    Spectrum reflectance;
    _fresnel->evaluateReflectance(VdotN, &reflectance);
    const real reflectionProbability = reflectance.average();
    const real sampleProbability     = Random::nextReal();
    
    bool isReflection = false;
    bool isRefraction = false;
    if (sampleProbability <= reflectionProbability) {
        isReflection = true;
    }
    else {
        isRefraction = true;
    }

    real etaI = _fresnel->iorOuter();
    real etaT = _fresnel->iorInner();
    Spectrum result(0.0_r);

    if (isReflection) {
        const real nFactor = (VdotN > 0.0_r) ? 1.0_r : -1.0_r;
        const Vector3R L = V.reflect(Ns * nFactor);

        const real pdf = reflectionProbability;

        surfaceIntersection.setWo(L);
        surfaceIntersection.setPdf(pdf);

        const real LdotN = L.absDot(Ns);

        const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
        Spectrum sampleSpectrum;
        _albedo->evaluate(uvw, &sampleSpectrum);

        result = sampleSpectrum * reflectance / LdotN;
    }
    else if (isRefraction) {
        Vector3R L;
        if (!V.canRefract(Ns, etaI, etaT, &L)) {
            return Spectrum(0.0_r);
        }
    
        const real cosThetaI = L.dot(Ns);
        if (cosThetaI < 0.0_r) {
            math::swap(etaI, etaT);
        }

        Spectrum refractDirectionReflectance;
        _fresnel->evaluateReflectance(cosThetaI, &refractDirectionReflectance);

        const Spectrum transmittance = refractDirectionReflectance.complement();
        const real btdfFactor = (etaT * etaT) / (etaI * etaI);
        const real pdf = 1.0_r - reflectionProbability;

        surfaceIntersection.setWo(L);
        surfaceIntersection.setPdf(pdf);

        const real LdotN = L.absDot(Ns);

        const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
        Spectrum sampleSpectrum;
        _albedo->evaluate(uvw, &sampleSpectrum);

        result = sampleSpectrum * transmittance * btdfFactor / LdotN;
    }
    else {
        // something wrong
    }

    return result;
}

real SpecularDielectric::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    return 0.0_r;
}

} // namespace cadise