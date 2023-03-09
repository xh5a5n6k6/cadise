#pragma once

#include "core/surface/microfacet/microfacet.h"

#include <memory>

namespace cadise { template<typename T> class TTexture; }

namespace cadise 
{

/*
    Reference Paper:
    "Microfacet Models for Refraction through Rough Surfaces"
    Walter et al., EGSR 2007

    "Understanding the Masking-Shadowing Function in Microfacet-Based BRDFs"
    Eric Heitz, JCGT 2014
*/
class AnisotropicGgx : public Microfacet 
{
public:
    AnisotropicGgx(
        const std::shared_ptr<TTexture<real>>& roughnessU,
        const std::shared_ptr<TTexture<real>>& roughnessV);

    real distributionD(
        const SurfaceIntersection& si,
        const Vector3R&            N,
        const Vector3R&            H) const override;

    real shadowingMaskingG(
        const SurfaceIntersection& si,
        const Vector3R&            V,
        const Vector3R&            L,
        const Vector3R&            N,
        const Vector3R&            H) const override;

    void sampleHalfVectorH(
        const SurfaceIntersection& si,
        const std::array<real, 2>& sample,
        Vector3R* const            out_H) const override;

private:
    real _lambda(
        const SurfaceIntersection& si,
        const real                 alphaX,
        const real                 alphaY,
        const Vector3R&            direction) const;

    std::shared_ptr<TTexture<real>> _roughnessU;
    std::shared_ptr<TTexture<real>> _roughnessV;
};

} // namespace cadise