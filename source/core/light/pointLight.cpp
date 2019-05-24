#include "core/light/pointLight.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

PointLight::PointLight(Vector3F position, Vector3F color) :
    _position(position), _color(color) {
}

Vector3F PointLight::evaluateSampleRadiance(Vector3F &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) {
    Vector3F offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.normal();
    Vector3F direction = _position - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();
    pdf = 1.0f;

    return _color;
}

Vector3F PointLight::position() {
    return _position;
}

Vector3F PointLight::color() {
    return _color;
}

} // namespace cadise