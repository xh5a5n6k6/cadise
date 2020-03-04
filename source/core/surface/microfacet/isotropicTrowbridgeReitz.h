#pragma once

#include "core/surface/microfacet/microfacet.h"

namespace cadise {

/*
    Reference Paper:
    "Microfacet Models for Refraction through Rough Surfaces"
    Walter et al., EGSR 2007
*/
class IsotropicTrowbridgeReitz : public Microfacet {
public:
    using Microfacet::Microfacet;

    real distributionD(
        const real      alphaX,
        const real      alphaY,
        const Vector3R& N,
        const Vector3R& H) const override;

    real shadowingMaskingG(
        const real      alphaX,
        const real      alphaY,
        const Vector3R& V,
        const Vector3R& L,
        const Vector3R& N,
        const Vector3R& H) const override;

    void sampleHalfVectorH(
        const real      alphaX,
        const real      alphaY,
        const Vector2R& sample,
        Vector3R* const out_H) const override;
};

} // namespace cadise