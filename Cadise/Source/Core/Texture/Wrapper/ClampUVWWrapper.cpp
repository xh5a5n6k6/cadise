#include "Core/Texture/Wrapper/ClampUVWWrapper.h"

#include "Foundation/Assertion.h"
#include "Math/TVector3.h"

namespace cadise
{

void ClampUVWWrapper::wrap(
    const Vector3R& uvw, 
    Vector3R* const out_wrapUvw) const 
{
    CS_ASSERT(out_wrapUvw);

    out_wrapUvw->set(uvw.clamp(0.0_r, 1.0_r));
}

} // namespace cadise