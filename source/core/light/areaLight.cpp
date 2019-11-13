#include "core/light/areaLight.h"

namespace cadise {

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