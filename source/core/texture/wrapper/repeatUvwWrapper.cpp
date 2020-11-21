#include "core/texture/wrapper/repeatUvwWrapper.h"

#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/tVector.h"

namespace cadise {

void RepeatUvwWrapper::wrap(const Vector3R& uvw, Vector3R* const out_wrapUvw) const {
    CADISE_ASSERT(out_wrapUvw);

    *out_wrapUvw = Vector3R(math::fractional(uvw.x()),
                            math::fractional(uvw.y()),
                            math::fractional(uvw.z()));
}

} // namespace cadise