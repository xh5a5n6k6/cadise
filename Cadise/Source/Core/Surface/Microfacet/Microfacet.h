#pragma once

#include "Math/Type/MathType.h"

#include <array>

// forward declaration
namespace cadise
{
    class SurfaceIntersection;
}

namespace cadise
{

class Microfacet
{
public:
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
        const std::array<real, 2>& sample,
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

    real _roughnessToAlpha(const real roughness) const;
};

} // namespace cadise