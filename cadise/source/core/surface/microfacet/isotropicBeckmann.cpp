#include "core/surface/microfacet/isotropicBeckmann.h"

#include "core/integral-tool/tSurfaceSampler.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/tVector3.h"

#include <cmath>

namespace cadise 
{

IsotropicBeckmann::IsotropicBeckmann(const std::shared_ptr<TTexture<real>>& roughness) :
    Microfacet(),
    _roughness(roughness) 
{
    CADISE_ASSERT(roughness);
}

real IsotropicBeckmann::distributionD(
    const SurfaceIntersection& si,
    const Vector3R&            N,
    const Vector3R&            H) const 
{
    const real NdotH = N.dot(H);
    if (NdotH <= 0.0_r)
    {
        return 0.0_r;
    }

    real sampleRoughness;
    TSurfaceSampler<real>().sample(si, _roughness.get(), &sampleRoughness);

    const real alpha  = _roughnessToAlpha(sampleRoughness);
    const real alpha2 = alpha * alpha;

    const real NdotH2 = NdotH * NdotH;
    const real NdotH4 = NdotH2 * NdotH2;

    const real exponent    = (NdotH2 - 1.0_r) / (NdotH2 * alpha2);
    const real denominator = constant::pi<real> * alpha2 * NdotH4;

    return std::exp(exponent) / denominator;
}

real IsotropicBeckmann::shadowingMaskingG(
    const SurfaceIntersection& si,
    const Vector3R&            V,
    const Vector3R&            L,
    const Vector3R&            N,
    const Vector3R&            H) const 
{
    if (!_isShadowingMaskingValid(V, L, N, H)) 
    {
        return 0.0_r;
    }

    real G1V;
    real G1L;

    real sampleRoughness;
    TSurfaceSampler<real>().sample(si, _roughness.get(), &sampleRoughness);

    const real alpha = _roughnessToAlpha(sampleRoughness);

    const real VdotN  = V.dot(N);
    const real VdotN2 = VdotN * VdotN;
    const real tanV   = std::sqrt((1.0_r - VdotN2) / VdotN2);
    const real aV     = 1.0_r / (alpha * tanV);
    if (aV < 1.6_r)
    {
        const real numerator   = 3.535_r * aV + 2.181_r * aV * aV;
        const real denominator = 1.0_r + 2.276_r * aV + 2.577_r * aV * aV;

        G1V = numerator / denominator;
    }
    else
    {
        G1V = 1.0_r;
    }

    const real LdotN  = L.dot(N);
    const real LdotN2 = LdotN * LdotN;
    const real tanL   = std::sqrt((1.0_r - LdotN2) / LdotN2);
    const real aL     = 1.0_r / (alpha * tanL);
    if (aL < 1.6_r) 
    {
        const real numerator   = 3.535_r * aL + 2.181_r * aL * aL;
        const real denominator = 1.0_r + 2.276_r * aL + 2.577_r * aL * aL;

        G1L = numerator / denominator;
    }
    else 
    {
        G1L = 1.0_r;
    }

    return G1V * G1L;
}

void IsotropicBeckmann::sampleHalfVectorH(
    const SurfaceIntersection& si,
    const std::array<real, 2>& sample,
    Vector3R* const            out_H) const 
{
    CADISE_ASSERT(out_H);
    
    // to avoid random sample with 1 value
    const std::array<real, 2> safeSample = {
        math::clamp(sample[0], 0.0_r, 0.9999_r),
        math::clamp(sample[1], 0.0_r, 0.9999_r) };

    real sampleRoughness;
    TSurfaceSampler<real>().sample(si, _roughness.get(), &sampleRoughness);

    const real alpha  = _roughnessToAlpha(sampleRoughness);
    const real alpha2 = alpha * alpha;
    
    const real phi       = constant::two_pi<real> * safeSample[0];
    const real tan2Theta = -alpha2 * std::log(1.0_r - safeSample[1]);
    const real cosTheta  = math::clamp(1.0_r / std::sqrt(1.0_r + tan2Theta), -1.0_r, 1.0_r);
    const real sinTheta  = std::sqrt(1.0_r - cosTheta * cosTheta);

    const Vector3R localH(
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta);

    // transform H to world coordinate
    const Vector3R worldH = si.surfaceDetail().shadingLcs().localToWorld(localH);

    out_H->set(worldH.normalize());
}

} // namespace cadise