#pragma once

#include "Math/Type/MathType.h"

namespace cadise
{

/*! Miscellaneous microfacet utilities. */
class MicrofacetUtility final
{
public:
    static bool canMakeReflectionH(
        const Vector3R& V,
        const Vector3R& L,
        const Vector3R& N,
        Vector3R* const out_H);

    static bool canMakeRefractionH(
        const Vector3R& V,
        const Vector3R& L,
        const Vector3R& N,
        const real      iorOuter,
        const real      iorInner,
        Vector3R* const out_H);
};

} // namespace cadise