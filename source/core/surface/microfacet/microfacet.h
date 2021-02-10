#pragma once

#include "math/type/mathType.h"

#include <memory>

namespace cadise {

class SurfaceIntersection;
template<typename T>
class TTexture;

class Microfacet {
public:
    Microfacet(const std::shared_ptr<TTexture<real>>& roughness);

    virtual ~Microfacet();

    virtual real distributionD(
        const SurfaceIntersection& si,
        const Vector3R&            N,
        const Vector3R&            H) const = 0;

    virtual real shadowingMaskingG(
        const SurfaceIntersection& si,
        const Vector3R&            V,
        const Vector3R&            L, 
        const Vector3R&            N, 
        const Vector3R&            H) const = 0;

    virtual void sampleHalfVectorH(
        const SurfaceIntersection& si,
        const Vector2R&            sample,
        Vector3R* const            out_H) const = 0;

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
    bool _isShadowingMaskingValid(
        const Vector3R& V,
        const Vector3R& L,
        const Vector3R& N,
        const Vector3R& H) const;

    std::shared_ptr<TTexture<real>> _roughness;
};

} // namespace cadise