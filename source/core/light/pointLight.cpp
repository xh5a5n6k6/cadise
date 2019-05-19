#include "core/light/pointLight.h"

#include "core/surfaceInfo.h"

namespace cadise {

PointLight::PointLight(Vector3 position, Vector3 color) :
    _position(position), _color(color) {
}

Vector3 PointLight::evaluateSampleRadiance(Vector3 &lightDirection, SurfaceInfo &surfaceInfo, float &t) {
    Vector3 direction = _position - surfaceInfo.hitPoint();
    t = direction.length();
    lightDirection = direction.normalize();

    return _color;
}

Vector3 PointLight::position() {
    return _position;
}

Vector3 PointLight::color() {
    return _color;
}

} // namespace cadise