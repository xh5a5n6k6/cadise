#include "Core/Texture/Wrapper/RepeatUVWWrapper.h"

#include "Foundation/Assertion.h"
#include "Math/Math.h"
#include "Math/TVector3.h"

namespace cadise
{

void RepeatUVWWrapper::wrap(
    const Vector3R& uvw,
    Vector3R* const out_wrapUvw) const
{
    CS_ASSERT(out_wrapUvw);

    out_wrapUvw->set(
        math::fractional(uvw.x()),
        math::fractional(uvw.y()),
        math::fractional(uvw.z()));
}

} // namespace cadise