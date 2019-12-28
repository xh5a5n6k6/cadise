#include "core/texture/wrapper/clampUvwWrapper.h"

#include "fundamental/assertion.h"
#include "math/vector.h"

namespace cadise {

void ClampUvwWrapper::wrap(const Vector3R& uvw, Vector3R* const out_wrapUvw) const {
    CADISE_ASSERT(out_wrapUvw);

    *out_wrapUvw = uvw.clamp(0.0_r, 1.0_r);
}

} // namespace cadise