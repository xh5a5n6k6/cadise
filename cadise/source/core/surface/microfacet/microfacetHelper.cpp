#include "core/surface/microfacet/microfacetHelper.h"

#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/tVector3.h"

#include <utility>

namespace cadise {

bool MicrofacetHelper::canMakeReflectionH(
    const Vector3R& V,
    const Vector3R& L,
    const Vector3R& N,
    Vector3R* const out_H) {

    CADISE_ASSERT(out_H);

    const Vector3R HVector = V.add(L);
    if (HVector.isZero()) {
        return false;
    }

    // make sure H and N lie in the same hemisphere
    const Vector3R H = HVector.normalize();
    switch (math::sign(N.dot(H))) {
        case constant::SIGN_POSITIVE:
            out_H->set(H);
            return true;

        case constant::SIGN_NEGATIVE:
            out_H->set(H.negate());
            return true;

        case constant::SIGN_ZERO:
            return false;
    }

    CADISE_ASSERT(false);

    return false;
}

bool MicrofacetHelper::canMakeRefractionH(
    const Vector3R& V,
    const Vector3R& L,
    const Vector3R& N,
    const real      iorOuter,
    const real      iorInner,
    Vector3R* const out_H) {

    CADISE_ASSERT(out_H);

    real etaI = iorOuter;
    real etaT = iorInner;
    if (L.dot(N) < 0.0_r) {
        std::swap(etaI, etaT);
    }

    const Vector3R HVector = V.mul(-etaT).add(L.mul(-etaI));
    if (HVector.isZero()) {
        return false;
    }

    // make sure H and N lie in the same hemisphere
    const Vector3R H = HVector.normalize();
    switch (math::sign(N.dot(H))) {
        case constant::SIGN_POSITIVE:
            out_H->set(H);
            return true;

        case constant::SIGN_NEGATIVE:
            out_H->set(H.negate());
            return true;

        case constant::SIGN_ZERO:
            return false;
    }

    CADISE_ASSERT(false);

    return false;
}

} // namespace cadise