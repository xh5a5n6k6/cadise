#include "core/surface/bsdf/conductorMicrofacet.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/surface/fresnel/conductorFresnel.h"
#include "core/surface/microfacet/microfacet.h"
#include "core/surface/microfacet/roughnessMapper.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/random.h"

#include <cmath>

namespace cadise {

ConductorMicrofacet::ConductorMicrofacet(
    const std::shared_ptr<Microfacet>&       microfacet,
    const std::shared_ptr<ConductorFresnel>& fresnel,
    const std::shared_ptr<Texture<real>>&    roughness) :
    
    Bsdf(BsdfLobes({ ELobe::GLOSSY_REFLECTION })),
    _microfacet(microfacet),
    _fresnel(fresnel),
    _roughness(roughness) {

    CADISE_ASSERT(microfacet);
    CADISE_ASSERT(fresnel);
    CADISE_ASSERT(roughness);
}

Spectrum ConductorMicrofacet::evaluate(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const {
    
    const Vector3R& Ns = si.surfaceInfo().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);

    if (VdotN * LdotN <= 0.0_r) {
        return Spectrum(0.0_r);
    }

    Vector3R H  = static_cast<real>(math::sign(VdotN)) * (V + L);
    if (H.isZero()) {
        return Spectrum(0.0_r);
    }
    H = H.normalize();

    const Vector3R& uvw = si.surfaceInfo().uvw();
    real sampleRoughness;
    _roughness->evaluate(uvw, &sampleRoughness);
    const real alpha = RoughnessMapper<RoughnessMapMode::SQUARE>::map(sampleRoughness);

    const real LdotH = L.dot(H);
    Spectrum F;
    _fresnel->evaluateReflectance(LdotH, &F);

    const real D = _microfacet->distributionD(alpha, alpha, Ns, H);
    const real G = _microfacet->shadowingMaskingG(alpha, alpha, V, L, Ns, H);

    return F * G * D / (4.0_r * std::abs(VdotN * LdotN));
}

void ConductorMicrofacet::evaluateSample(
    const TransportInfo&       info, 
    const SurfaceIntersection& si,
    BsdfSample* const          out_sample) const {

    CADISE_ASSERT(out_sample);

    const Vector3R& Ns      = si.surfaceInfo().shadingNormal();
    const Vector3R& V       = si.wi();
    const real      Nfactor = (V.dot(Ns) > 0.0_r) ? 1.0_r : -1.0_r;

    const Vector3R& uvw = si.surfaceInfo().uvw();
    real sampleRoughness;
    _roughness->evaluate(uvw, &sampleRoughness);
    const real alpha = RoughnessMapper<RoughnessMapMode::SQUARE>::map(sampleRoughness);

    // build local coordinate system (shading normal as y-axis)
    const Vector3R yAxis(Ns);
    Vector3R zAxis;
    Vector3R xAxis;
    math::build_coordinate_system(yAxis, &zAxis, &xAxis);

    const Vector2R sample(Random::nextReal(), Random::nextReal());
    Vector3R H;
    _microfacet->sampleHalfVectorH(alpha, alpha, sample, &H);

    // transform H to world coordinate
    H = xAxis * H.x() + yAxis * H.y() + zAxis * H.z();
    H = H.normalize();

    const Vector3R L = V.reflect(H * Nfactor);

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);
    const real LdotH = L.dot(H);
    const real NdotH = Ns.dot(H);

    if (VdotN * LdotN <= 0.0_r) {
        return;
    }

    Spectrum F;
    _fresnel->evaluateReflectance(LdotH, &F);

    const real D = _microfacet->distributionD(alpha, alpha, Ns, H);
    const real G = _microfacet->shadowingMaskingG(alpha, alpha, V, L, Ns, H);

    const real pdfH = std::abs(D * NdotH);
    const real pdfL = std::abs(pdfH / (4.0_r * LdotH));
    if (!std::isfinite(pdfL)) {
        return;
    }

    out_sample->setScatterValue(F * G * D / (4.0_r * std::abs(VdotN * LdotN)));
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfL);
}

real ConductorMicrofacet::evaluatePdfW(
    const TransportInfo&       info, 
    const SurfaceIntersection& si) const {
    
    const Vector3R& Ns = si.surfaceInfo().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);

    if (VdotN * LdotN <= 0.0_r) {
        return 0.0_r;
    }

    Vector3R H = static_cast<real>(math::sign(VdotN)) * (V + L);
    if (H.isZero()) {
        return 0.0_r;
    }
    H = H.normalize();

    const Vector3R& uvw = si.surfaceInfo().uvw();
    real sampleRoughness;
    _roughness->evaluate(uvw, &sampleRoughness);
    const real alpha = RoughnessMapper<RoughnessMapMode::SQUARE>::map(sampleRoughness);

    const real D     = _microfacet->distributionD(alpha, alpha, Ns, H);
    const real NdotH = Ns.dot(H);
    const real LdotH = L.dot(H);

    const real pdfH = std::abs(D * NdotH);
    const real pdfL = std::abs(pdfH / (4.0_r * LdotH));
    if (!std::isfinite(pdfL)) {
        return 0.0_r;
    }

    return pdfL;
}

} // namespace cadise