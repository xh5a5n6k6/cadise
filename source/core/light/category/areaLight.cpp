#include "core/light/category/areaLight.h"

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

} // namespace cadise