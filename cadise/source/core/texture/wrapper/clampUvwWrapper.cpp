#include "core/texture/wrapper/clampUvwWrapper.h"

#include "fundamental/assertion.h"
#include "math/tVector3.h"

namespace cadise {

void ClampUvwWrapper::wrap(const Vector3R& uvw, Vector3R* const out_wrapUvw) const {
    CADISE_ASSERT(out_wrapUvw);

    out_wrapUvw->set(uvw.clamp(0.0_r, 1.0_r));
}

} // namespace cadise