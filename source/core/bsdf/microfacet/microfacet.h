#pragma once

#include "math/type/mathType.h"

namespace cadise {

class Microfacet {
public:
    Microfacet();

    virtual ~Microfacet();

    virtual real distributionD(const real alphaX, 
                               const real alphaY,
                               const Vector3R& N,
                               const Vector3R& H) const = 0;

    virtual real shadowingMaskingG(const real alphaX, 
                                   const real alphaY,
                                   const Vector3R& V,
                                   const Vector3R& L, 
                                   const Vector3R& N, 
                                   const Vector3R& H) const = 0;

    virtual void sampleHalfVectorH(const real alphaX,
                                   const real alphaY,
                                   const Vector2R& sample,
                                   Vector3R* const out_H) const = 0;

protected:
    /*
        The back surface of the microsurface is never visible from
        directions on the front side of the macrosurface and vice 
        versa (sidedness agreement)

        Reference Paper:
        "Microfacet Models for Refraction through Rough Surfaces"
        Walter et al., EGSR 2007

        Equation (7)
    */
    bool _isShadowingMaskingValid(const Vector3R& V,
                                  const Vector3R& L,
                                  const Vector3R& N,
                                  const Vector3R& H) const;
};

} // namespace cadise