#include "core/surface/bsdf/specularDielectric.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/surface/fresnel/dielectricFresnel.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/random.h"

#include <cmath>

namespace cadise {

SpecularDielectric::SpecularDielectric(
    const std::shared_ptr<Texture<Spectrum>>& albedo, 
    const std::shared_ptr<DielectricFresnel>& fresnel) :
    
    Bsdf(BsdfType(BxdfType::SPECULAR_REFLECTION) | BsdfType(BxdfType::SPECULAR_TRANSMISSION)),
    _albedo(albedo),
    _fresnel(fresnel) {

    CADISE_ASSERT(albedo);
    CADISE_ASSERT(fresnel);
}

Spectrum SpecularDielectric::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {

    return Spectrum(0.0_r);
}

void SpecularDielectric::evaluateSample(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection,
    BsdfSample* const          out_sample) const {

    CADISE_ASSERT(out_sample);

    const Vector3R& Ns    = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V     = surfaceIntersection.wi();
    const real      VdotN = V.dot(Ns);
    
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

    real     scatterPdfW = 0.0_r;
    Vector3R scatterDirection(0.0_r);
    Spectrum scatterValue(0.0_r);

    if (isReflection) {
        const real     Nfactor = (VdotN > 0.0_r) ? 1.0_r : -1.0_r;
        const Vector3R L       = V.reflect(Ns * Nfactor);
        const real     LdotN   = L.absDot(Ns);

        const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
        Spectrum sampleSpectrum;
        _albedo->evaluate(uvw, &sampleSpectrum);

        scatterValue     = sampleSpectrum * reflectance / LdotN;
        scatterDirection = L;
        scatterPdfW      = reflectionProbability;
    }
    else if (isRefraction) {
        real etaI = _fresnel->iorOuter();
        real etaT = _fresnel->iorInner();
        
        Vector3R L;
        if (!V.canRefract(Ns, etaI, etaT, &L)) {
            return;
        }
    
        const real cosThetaI = L.dot(Ns);
        Spectrum   refractDirectionReflectance;
        _fresnel->evaluateReflectance(cosThetaI, &refractDirectionReflectance);

        real btdfFactor = 1.0_r;
        if (transportInfo.mode() == TransportMode::RADIANCE) {
            if (cosThetaI < 0.0_r) {
                math::swap(etaI, etaT);
            }

            btdfFactor = (etaT * etaT) / (etaI * etaI);
        }

        const real     LdotN         = std::abs(cosThetaI);
        const Spectrum transmittance = refractDirectionReflectance.complement();

        const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
        Spectrum sampleSpectrum;
        _albedo->evaluate(uvw, &sampleSpectrum);

        scatterValue     = sampleSpectrum * transmittance * btdfFactor / LdotN;
        scatterDirection = L;
        scatterPdfW      = 1.0_r - reflectionProbability;
    }
    else {
        // something wrong
    }

    out_sample->setScatterValue(scatterValue);
    out_sample->setScatterDirection(scatterDirection);
    out_sample->setPdfW(scatterPdfW);
}

real SpecularDielectric::evaluatePdfW(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {

    return 0.0_r;
}

} // namespace cadise