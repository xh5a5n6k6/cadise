#include "core/bsdf/category/conductorMicrofacet.h"

#include "core/bsdf/fresnel/conductorFresnel.h"
#include "core/bsdf/microfacet/microfacet.h"
#include "core/bsdf/microfacet/roughnessMapper.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/random.h"

#include <cmath>

namespace cadise {

ConductorMicrofacet::ConductorMicrofacet(const std::shared_ptr<Microfacet>&       microfacet,
                                         const std::shared_ptr<ConductorFresnel>& fresnel,
                                         const std::shared_ptr<Texture<real>>&    roughness) :
    Bsdf(BsdfType(BxdfType::GLOSSY_REFLECTION)),
    _microfacet(microfacet),
    _fresnel(fresnel),
    _roughness(roughness) {

    CADISE_ASSERT(microfacet);
    CADISE_ASSERT(fresnel);
    CADISE_ASSERT(roughness);
}

Spectrum ConductorMicrofacet::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R V  = surfaceIntersection.wi();
    const Vector3R L  = surfaceIntersection.wo();

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

    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    real sampleRoughness;
    _roughness->evaluate(uvw, &sampleRoughness);

    const real alpha = RoughnessMapper<RoughnessMapMode::SQUARE>::map(sampleRoughness);

    const real LdotH = L.dot(H);
    Spectrum F;
    _fresnel->evaluateReflectance(LdotH, &F);

    const real D = _microfacet->distributionD(alpha, alpha, Ns, H);
    const real G = _microfacet->shadowingMaskingG(alpha, alpha, V, L, Ns, H);

    const Spectrum numerator   = F * G * D;
    const real     denominator = 4.0_r * std::abs(VdotN * LdotN);

    return numerator / denominator;
}

Spectrum ConductorMicrofacet::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R V  = surfaceIntersection.wi();

    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
    real sampleRoughness;
    _roughness->evaluate(uvw, &sampleRoughness);

    const real alpha = RoughnessMapper<RoughnessMapMode::SQUARE>::map(sampleRoughness);

    // build local coordinate system (shading normal as z-axis)
    const Vector3R zAxis(Ns);
    Vector3R xAxis;
    Vector3R yAxis;
    math::buildCoordinateSystem(zAxis, &xAxis, &yAxis);

    const Vector2R sample(Random::nextReal(), Random::nextReal());
    Vector3R H;
    _microfacet->sampleHalfVectorH(alpha, alpha, sample, &H);

    // transform H to world coordinate
    H = xAxis * H.x() + yAxis * H.y() + zAxis * H.z();
    H = H.normalize();

    const Vector3R L = V.reflect(H);

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);
    const real LdotH = L.dot(H);
    const real NdotH = Ns.dot(H);

    if (VdotN * LdotN <= 0.0_r) {
        return Spectrum(0.0_r);
    }

    Spectrum F;
    _fresnel->evaluateReflectance(LdotH, &F);

    const real D = _microfacet->distributionD(alpha, alpha, Ns, H);
    const real G = _microfacet->shadowingMaskingG(alpha, alpha, V, L, Ns, H);

    const real pdfH = std::abs(D * NdotH);
    const real pdfL = std::abs(pdfH / (4.0_r * LdotH));
    if (!std::isfinite(pdfL)) {
        return Spectrum(0.0_r);
    }

    const Spectrum numerator   = F * G * D;
    const real     denominator = 4.0_r * std::abs(VdotN * LdotN);

    surfaceIntersection.setWo(L);
    surfaceIntersection.setPdf(pdfL);

    return numerator / denominator;
}

real ConductorMicrofacet::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R V  = surfaceIntersection.wi();
    const Vector3R L  = surfaceIntersection.wo();

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

    const Vector3R uvw = surfaceIntersection.surfaceInfo().uvw();
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