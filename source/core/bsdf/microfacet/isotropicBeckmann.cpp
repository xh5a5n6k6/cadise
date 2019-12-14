#include "core/bsdf/microfacet/isotropicBeckmann.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

real IsotropicBeckmann::distributionD(const real alphaX,
                                      const real alphaY,
                                      const Vector3R& N,
                                      const Vector3R& H) const {

    real NdotH = N.dot(H);
    if (NdotH <= 0.0_r) {
        return 0.0_r;
    }

    // for isotropic microfacet alphaX is equal to alphaY,
    // we can use either of them as alpha.
    const real alpha  = alphaX;
    const real alpha2 = alpha * alpha;
    const real NdotH2 = NdotH * NdotH;
    const real NdotH4 = NdotH2 * NdotH2;

    const real exponent    = (NdotH2 - 1.0_r) / (NdotH2 * alpha2);
    const real denominator = constant::PI * alpha2 * NdotH4;

    return std::exp(exponent) / denominator;
}

real IsotropicBeckmann::shadowingMaskingG(const real alphaX,
                                          const real alphaY,
                                          const Vector3R& V,
                                          const Vector3R& L,
                                          const Vector3R& N,
                                          const Vector3R& H) const {

    if (!_isShadowingMaskingValid(V, L, N, H)) {
        return 0.0_r;
    }

    real G1V;
    real G1L;

    // for isotropic microfacet alphaX is equal to alphaY,
    // we can use either of them as alpha.
    const real alpha = alphaX;

    const real VdotN  = V.dot(N);
    const real VdotN2 = VdotN * VdotN;
    const real tanV   = std::sqrt((1.0_r - VdotN2) / VdotN2);
    const real aV     = 1.0_r / (alpha * tanV);
    if (aV < 1.6_r) {
        const real numerator   = 3.535_r * aV + 2.181_r * aV * aV;
        const real denominator = 1.0_r + 2.276_r * aV + 2.577_r * aV * aV;

        G1V = numerator / denominator;
    }
    else {
        G1V = 1.0_r;
    }

    const real LdotN  = L.dot(N);
    const real LdotN2 = LdotN * LdotN;
    const real tanL   = std::sqrt((1.0_r - LdotN2) / LdotN2);
    const real aL     = 1.0_r / (alpha * tanL);
    if (aL < 1.6_r) {
        const real numerator   = 3.535_r * aL + 2.181_r * aL * aL;
        const real denominator = 1.0_r + 2.276_r * aL + 2.577_r * aL * aL;

        G1L = numerator / denominator;
    }
    else {
        G1L = 1.0_r;
    }

    return G1V * G1L;
}

void IsotropicBeckmann::sampleHalfVectorH(const real alphaX,
                                          const real alphaY,
                                          const Vector2R& sample,
                                          Vector3R* const out_H) const {

    CADISE_ASSERT(out_H);
    
    // to avoid random sample with 1 value
    const Vector2R safeSample = sample.clamp(0.0_r, 0.9999_r);

    // for isotropic microfacet alphaX is equal to alphaY,
    // we can use either of them as alpha.
    const real alpha  = alphaX;
    const real alpha2 = alpha * alpha;
    
    const real phi   = constant::TWO_PI * safeSample.x();
    const real theta = std::atan(std::sqrt(-alpha2 * std::log(1.0_r - safeSample.y())));
    if (!std::isfinite(theta)) {
        *out_H = Vector3R(0.0_r, 0.0_r, 1.0_r);

        return;
    }

    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sin(theta);
    const real cosPhi = std::cos(phi);
    const real sinPhi = std::sin(phi);

    *out_H = Vector3R(cosPhi * sinTheta,
                      sinPhi * sinTheta,
                      cosTheta);
}

} // namespace cadise