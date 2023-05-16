#include "Core/Surface/Microfacet/IsotropicBlinnPhong.h"

#include "Core/Gear/TSurfaceSampler.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/Math.h"
#include "Math/TVector3.h"

#include <cmath>

namespace cadise
{

IsotropicBlinnPhong::IsotropicBlinnPhong(const std::shared_ptr<TTexture<real>>& roughness) :
    Microfacet(),
    _roughness(roughness)
{
    CS_ASSERT(roughness);
}

real IsotropicBlinnPhong::distributionD(
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
    const real alphaP = 2.0_r / alpha2 - 2.0_r;

    return (alphaP + 2.0_r) * std::pow(NdotH, alphaP) * constant::rcp_two_pi<real>;
}

real IsotropicBlinnPhong::shadowingMaskingG(
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
    const real alphaP = 2.0_r / alpha2 - 2.0_r;

    const real VdotN  = V.dot(N);
    const real VdotN2 = VdotN * VdotN;
    const real tanV2  = (1.0_r - VdotN2) / VdotN2;
    const real aV     = std::sqrt((0.5_r * alphaP + 1.0_r) / tanV2);
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
    const real tanL2  = (1.0_r - LdotN2) / LdotN2;
    const real aL     = std::sqrt((0.5_r * alphaP + 1.0_r) / tanL2);
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

void IsotropicBlinnPhong::sampleHalfVectorH(
    const SurfaceIntersection& si,
    const std::array<real, 2>& sample,
    Vector3R* const            out_H) const
{
    CS_ASSERT(out_H);

    real sampleRoughness;
    TSurfaceSampler<real>().sample(si, _roughness.get(), &sampleRoughness);

    const real alpha  = _roughnessToAlpha(sampleRoughness);
    const real alpha2 = alpha * alpha;
    const real alphaP = 2.0_r / alpha2 - 2.0_r;

    const real phi      = constant::two_pi<real> * sample[0];
    const real cosTheta = std::pow(sample[1], 1.0_r / (alphaP + 2.0_r));
    const real sinTheta = std::sqrt(1.0_r - cosTheta * cosTheta);

    const Vector3R localH(
        std::sin(phi) * sinTheta,
        cosTheta,
        std::cos(phi) * sinTheta);

    // transform H to world coordinate
    const Vector3R worldH = si.surfaceDetail().shadingLcs().localToWorld(localH);

    out_H->set(worldH.normalize());
}

} // namespace cadise