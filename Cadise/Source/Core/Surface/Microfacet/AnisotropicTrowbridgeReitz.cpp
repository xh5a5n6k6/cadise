#include "Core/Surface/Microfacet/AnisotropicTrowbridgeReitz.h"

#include "Core/Gear/TSurfaceSampler.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/Math.h"
#include "Math/TVector3.h"

#include <cmath>

namespace cadise
{

AnisotropicTrowbridgeReitz::AnisotropicTrowbridgeReitz(
    const std::shared_ptr<TTexture<real>>& roughnessU,
    const std::shared_ptr<TTexture<real>>& roughnessV) :

    Microfacet(),
    _roughnessU(roughnessU),
    _roughnessV(roughnessV)
{
    CS_ASSERT(roughnessU);
    CS_ASSERT(roughnessV);
}

real AnisotropicTrowbridgeReitz::distributionD(
    const SurfaceIntersection& si,
    const Vector3R&            N,
    const Vector3R&            H) const 
{
    const real cosTheta = math::clamp(N.dot(H), -1.0_r, 1.0_r);
    if (cosTheta <= 0.0_r) 
    {
        return 0.0_r;
    }

    real sampleRoughnessU;
    TSurfaceSampler<real>().sample(si, _roughnessU.get(), &sampleRoughnessU);

    real sampleRoughnessV;
    TSurfaceSampler<real>().sample(si, _roughnessV.get(), &sampleRoughnessV);

    const real alphaX  = _roughnessToAlpha(sampleRoughnessU);
    const real alphaY  = _roughnessToAlpha(sampleRoughnessV);
    const real alphaX2 = alphaX * alphaX;
    const real alphaY2 = alphaY * alphaY;

    const real cos2Theta = cosTheta * cosTheta;
    const real tan2Theta = (1.0_r - cos2Theta) / cos2Theta;
    const real cos2Phi   = si.surfaceDetail().shadingLcs().cos2Phi(H);
    const real sin2Phi   = 1.0_r - cos2Phi;

    const real squareFactor = cos2Theta * (1.0_r + tan2Theta * (cos2Phi / alphaX2 + sin2Phi / alphaY2));
    const real denominator  = constant::pi<real> * alphaX * alphaY * (squareFactor * squareFactor);
    
    return 1.0_r / denominator;
}

real AnisotropicTrowbridgeReitz::shadowingMaskingG(
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

    real sampleRoughnessU;
    TSurfaceSampler<real>().sample(si, _roughnessU.get(), &sampleRoughnessU);

    real sampleRoughnessV;
    TSurfaceSampler<real>().sample(si, _roughnessV.get(), &sampleRoughnessV);

    const real alphaX = _roughnessToAlpha(sampleRoughnessU);
    const real alphaY = _roughnessToAlpha(sampleRoughnessV);

    const real lambdaV = _lambda(si, alphaX, alphaY, V);
    const real lambdaL = _lambda(si, alphaX, alphaY, L);

    return 1.0_r / (1.0_r + lambdaV + lambdaL);
}

void AnisotropicTrowbridgeReitz::sampleHalfVectorH(
    const SurfaceIntersection& si,
    const std::array<real, 2>& sample,
    Vector3R* const            out_H) const 
{
    CS_ASSERT(out_H);

    // to avoid random sample with 1 value
    const std::array<real, 2> safeSample = {
        math::clamp(sample[0], 0.0_r, 0.9999_r),
        math::clamp(sample[1], 0.0_r, 0.9999_r) };

    real sampleRoughnessU;
    TSurfaceSampler<real>().sample(si, _roughnessU.get(), &sampleRoughnessU);

    real sampleRoughnessV;
    TSurfaceSampler<real>().sample(si, _roughnessV.get(), &sampleRoughnessV);

    const real alphaX  = _roughnessToAlpha(sampleRoughnessU);
    const real alphaY  = _roughnessToAlpha(sampleRoughnessV);
    const real alphaX2 = alphaX * alphaX;
    const real alphaY2 = alphaY * alphaY;

    /*
        implementation follows Jiayin Cao's solution to offset sample phi.
        Reference Note: https://agraphicsguynotes.com/posts/sample_anisotropic_microfacet_brdf/
    */
    const real phiRaw = std::atan(alphaY / alphaX * std::tan(constant::two_pi<real> * safeSample[0]));

    real phi = phiRaw;
    if (safeSample[0] >= 0.75_r)
    {
        phi += constant::two_pi<real>;
    }
    else if (safeSample[0] > 0.25_r)
    {
        phi += constant::pi<real>;
    }

    const real cosPhi         = std::cos(phi);
    const real sinPhi         = std::sin(phi);
    const real inverseAlphaO2 = 1.0_r / (cosPhi * cosPhi / alphaX2 + sinPhi * sinPhi / alphaY2);
    const real tan2Theta      = inverseAlphaO2 * safeSample[1] / (1.0_r - safeSample[1]);
    const real cosTheta       = math::clamp(1.0_r / std::sqrt(1.0_r + tan2Theta), -1.0_r, 1.0_r);
    const real sinTheta       = std::sqrt(1.0_r - cosTheta * cosTheta);

    const Vector3R localH(
        sinPhi * sinTheta,
        cosTheta,
        cosPhi * sinTheta);

    // transform H to world coordinate
    const Vector3R worldH = si.surfaceDetail().shadingLcs().localToWorld(localH);

    out_H->set(worldH.normalize());
}

real AnisotropicTrowbridgeReitz::_lambda(
    const SurfaceIntersection& si,
    const real                 alphaX,
    const real                 alphaY,
    const Vector3R&            direction) const
{
    const real tan2Theta = si.surfaceDetail().shadingLcs().tan2Theta(direction);
    const real cos2Phi   = si.surfaceDetail().shadingLcs().cos2Phi(direction);
    const real sin2Phi   = 1.0_r - cos2Phi;

    const real alphaO2       = cos2Phi * alphaX * alphaX + sin2Phi * alphaY * alphaY;
    const real inverseAlpha2 = alphaO2 * tan2Theta;

    return (-1.0_r + std::sqrt(1.0_r + inverseAlpha2)) * 0.5_r;
}

} // namespace cadise