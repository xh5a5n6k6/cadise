#include "core/surface/microfacet/isotropicTrowbridgeReitz.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

real IsotropicTrowbridgeReitz::distributionD(
    const real      alphaX,
    const real      alphaY,
    const Vector3R& N,
    const Vector3R& H) const {

    const real NdotH = N.dot(H);
    if (NdotH <= 0.0_r) {
        return 0.0_r;
    }

    // for isotropic microfacet alphaX is equal to alphaY,
    // we can use either of them as alpha.
    const real alpha  = alphaX;
    const real alpha2 = alpha * alpha;
    const real NdotH2 = NdotH * NdotH;
    const real NdotH4 = NdotH2 * NdotH2;
    const real tan2   = (1.0_r - NdotH2) / NdotH2;

    const real numerator   = alpha2;
    const real denominator = constant::pi<real> * NdotH4 * (alpha2 + tan2) * (alpha2 + tan2);

    return numerator / denominator;
}

real IsotropicTrowbridgeReitz::shadowingMaskingG(
    const real      alphaX,
    const real      alphaY,
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
    const real alpha  = alphaX;
    const real alpha2 = alpha * alpha;

    const real VdotN  = V.dot(N);
    const real VdotN2 = VdotN * VdotN;
    const real tanV2  = (1.0_r - VdotN2) / VdotN2;

    G1V = 2.0_r / (1.0_r + std::sqrt(1.0_r + alpha2 * tanV2));

    const real LdotN  = L.dot(N);
    const real LdotN2 = LdotN * LdotN;
    const real tanL2  = (1.0_r - LdotN2) / LdotN2;

    G1L = 2.0_r / (1.0_r + std::sqrt(1.0_r + alpha2 * tanL2));

    return G1V * G1L;
}

void IsotropicTrowbridgeReitz::sampleHalfVectorH(
    const real      alphaX,
    const real      alphaY,
    const Vector2R& sample,
    Vector3R* const out_H) const {

    CADISE_ASSERT(out_H);

    // to avoid random sample with 1 value
    const Vector2R safeSample = sample.clamp(0.0_r, 0.9999_r);

    // for isotropic microfacet alphaX is equal to alphaY,
    // we can use either of them as alpha.
    const real alpha = alphaX;

    const real phi   = constant::two_pi<real> * safeSample.x();
    const real theta = std::atan(alpha * std::sqrt(safeSample.y() / (1.0_r - safeSample.y())));
    if (!std::isfinite(theta)) {
        *out_H = Vector3R(0.0_r, 1.0_r, 0.0_r);

        return;
    }

    const real cosTheta = std::cos(theta);
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_H = Vector3R(std::sin(phi) * sinTheta,
                      cosTheta,
                      std::cos(phi) * sinTheta);
}

} // namespace cadise