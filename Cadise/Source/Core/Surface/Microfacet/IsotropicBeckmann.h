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
*/
class IsotropicBeckmann : public Microfacet 
{
public:
    IsotropicBeckmann(const std::shared_ptr<TTexture<real>>& roughness);

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
    std::shared_ptr<TTexture<real>> _roughness;
};

} // namespace cadise