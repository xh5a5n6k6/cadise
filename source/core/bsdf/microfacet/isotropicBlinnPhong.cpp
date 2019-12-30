#include "core/bsdf/microfacet/isotropicBlinnPhong.h"

#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/vector.h"

#include <cmath>

namespace cadise {

real IsotropicBlinnPhong::distributionD(const real alphaX,
                                        const real alphaY,
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
    const real alphaP = 2.0_r / alpha2 - 2.0_r;

    return (alphaP + 2.0_r) * std::pow(NdotH, alphaP) * constant::INV_TWO_PI;
}

real IsotropicBlinnPhong::shadowingMaskingG(const real alphaX,
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
    const real alpha  = alphaX;
    const real alpha2 = alpha * alpha;
    const real alphaP = 2.0_r / alpha2 - 2.0_r;

    const real VdotN  = V.dot(N);
    const real VdotN2 = VdotN * VdotN;
    const real tanV2  = (1.0_r - VdotN2) / VdotN2;
    const real aV     = std::sqrt((0.5_r * alphaP + 1.0_r) / tanV2);
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
    const real tanL2  = (1.0_r - LdotN2) / LdotN2;
    const real aL     = std::sqrt((0.5_r * alphaP + 1.0_r) / tanL2);
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

void IsotropicBlinnPhong::sampleHalfVectorH(const real alphaX,
                                            const real alphaY,
                                            const Vector2R& sample,
                                            Vector3R* const out_H) const {

    CADISE_ASSERT(out_H);

    // for isotropic microfacet alphaX is equal to alphaY,
    // we can use either of them as alpha.
    const real alpha  = alphaX;
    const real alpha2 = alpha * alpha;
    const real alphaP = 2.0_r / alpha2 - 2.0_r;

    const real phi      = constant::TWO_PI * sample.x();
    const real cosPhi   = std::cos(phi);
    const real sinPhi   = std::sin(phi);
    const real cosTheta = std::pow(sample.y(), 1.0_r / (alphaP + 2.0_r));
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    *out_H = Vector3R(cosPhi * sinTheta,
                      sinPhi * sinTheta,
                      cosTheta);
}

} // namespace cadise