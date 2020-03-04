#include "core/surface/bsdf/specularTransmission.h"

#include "core/surface/fresnel/dielectricFresnel.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
#include "math/math.h"

namespace cadise {


SpecularTransmission::SpecularTransmission(
    const std::shared_ptr<Texture<Spectrum>>& albedo,
    const std::shared_ptr<DielectricFresnel>& fresnel) :
    
    Bsdf(BsdfType(BxdfType::SPECULAR_TRANSMISSION)),
    _albedo(albedo),
    _fresnel(fresnel) {

    CADISE_ASSERT(albedo);
    CADISE_ASSERT(fresnel);
}

Spectrum SpecularTransmission::evaluate(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection) const {
    
    return Spectrum(0.0_r);
}

Spectrum SpecularTransmission::evaluateSample(
    const TransportInfo& transportInfo, 
    SurfaceIntersection& surfaceIntersection) const {
    
    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();

    real etaI = _fresnel->iorOuter();
    real etaT = _fresnel->iorInner();

    Vector3R L;
    if (!V.canRefract(Ns, etaI, etaT, &L)) {
        return Spectrum(0.0_r);
    }

    const real cosThetaI = L.dot(Ns);

    Spectrum reflectance;
    _fresnel->evaluateReflectance(cosThetaI, &reflectance);

    real btdfFactor = 1.0_r;
    if (transportInfo.mode() == TransportMode::RADIANCE) {
        if (cosThetaI < 0.0_r) {
            math::swap(etaI, etaT);
        }

        btdfFactor = (etaT * etaT) / (etaI * etaI);
    }

    surfaceIntersection.setWo(L);
    surfaceIntersection.setPdf(1.0_r);

    const real LdotN = L.absDot(Ns);
    const Spectrum transmittance = reflectance.complement();

    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * transmittance * btdfFactor / LdotN;
}

real SpecularTransmission::evaluatePdfW(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection) const {
    
    return 0.0_r;
}

} // namespace cadise