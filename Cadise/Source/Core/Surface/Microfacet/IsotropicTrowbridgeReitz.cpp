#include "Core/Surface/Microfacet/IsotropicTrowbridgeReitz.h"

#include "Core/Gear/TSurfaceSampler.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/Math.h"
#include "Math/TVector3.h"

#include <cmath>

namespace cadise 
{

IsotropicTrowbridgeReitz::IsotropicTrowbridgeReitz(const std::shared_ptr<TTexture<real>>& roughness) :
    Microfacet(),
    _roughness(roughness)
{
    CS_ASSERT(roughness);
}

real IsotropicTrowbridgeReitz::distributionD(
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
    const real tan2   = (1.0_r - NdotH2) / NdotH2;

    const real numerator   = alpha2;
    const real denominator = constant::pi<real> * NdotH4 * (alpha2 + tan2) * (alpha2 + tan2);

    return numerator / denominator;
}

real IsotropicTrowbridgeReitz::shadowingMaskingG(
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

    const real alpha  = _roughnessToAlpha(sampleRoughness);
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
    const SurfaceIntersection& si,
    const std::array<real, 2>& sample,
    Vector3R* const            out_H) const 
{
    CS_ASSERT(out_H);

    // to avoid random sample with 1 value
    const std::array<real, 2> safeSample = {
        math::clamp(sample[0], 0.0_r, 0.9999_r),
        math::clamp(sample[1], 0.0_r, 0.9999_r) };

    real sampleRoughness;
    TSurfaceSampler<real>().sample(si, _roughness.get(), &sampleRoughness);

    const real alpha = _roughnessToAlpha(sampleRoughness);

    const real phi       = constant::two_pi<real> * safeSample[0];
    const real tan2Theta = alpha * alpha * (safeSample[1] / (1.0_r - safeSample[1]));
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