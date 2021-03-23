#include "core/surface/microfacet/microfacetHelper.h"

#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/tVector.h"

namespace cadise {

bool MicrofacetHelper::canMakeReflectionH(
    const Vector3R& V,
    const Vector3R& L,
    const Vector3R& N,
    Vector3R* const out_H) {

    CADISE_ASSERT(out_H);

    const Vector3R HVector = V + L;
    if (HVector.isZero()) {
        return false;
    }

    // make sure H and N lie in the same hemisphere
    const Vector3R H = HVector.normalize();
    switch (math::sign(H.dot(N))) {
        case constant::SIGN_POSITIVE:
            *out_H = H;
            return true;

        case constant::SIGN_NEGATIVE:
            *out_H = -H;
            return true;

        case constant::SIGN_ZERO:
            return false;
    }

    CADISE_ASSERT(false);

    return false;
}

} // namespace cadise