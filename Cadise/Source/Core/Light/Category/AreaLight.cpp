#include "Core/Light/Category/AreaLight.h"

#include "Foundation/Assertion.h"
#include "Math/TVector3.h"

namespace cadise 
{

AreaLight::AreaLight() :
    AreaLight(false)
{}

AreaLight::AreaLight(const bool isBackFaceEmit) :
    Light(),
    _isBackFaceEmit(isBackFaceEmit)
{}

bool AreaLight::isDeltaLight() const 
{
    return false;
}

void AreaLight::setIsBackFaceEmit(const bool isBackFaceEmit)
{
    _isBackFaceEmit = isBackFaceEmit;
}

bool AreaLight::_canEmit(const Vector3R& direction, const Vector3R& N) const 
{
    CS_ASSERT_GT(N.length(), 0.0_r);

    return (direction.dot(N) >= 0.0_r) != _isBackFaceEmit;
}

real AreaLight::_defaultFlux() const
{
    return 1.0_r;
}

} // namespace cadise