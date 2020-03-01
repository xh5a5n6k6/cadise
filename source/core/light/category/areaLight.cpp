#include "core/light/category/areaLight.h"

#include "fundamental/assertion.h"

namespace cadise {

AreaLight::AreaLight() :
    AreaLight(false) {
}

AreaLight::AreaLight(const bool isBackFaceEmit) :
    _isBackFaceEmit(isBackFaceEmit) {
}

bool AreaLight::isDeltaLight() const {
    return false;
}

void AreaLight::setIsBackFaceEmit(const bool isBackFaceEmit) {
    _isBackFaceEmit = isBackFaceEmit;
}

bool AreaLight::_canEmit(const Vector3R& direction, const Vector3R& N) const {
    CADISE_ASSERT_GT(N.length(), 0.0_r);

    return (direction.dot(N) >= 0.0_r) != _isBackFaceEmit;
}

} // namespace cadise