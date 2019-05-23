#include "core/light/pointLight.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

PointLight::PointLight(Vector3 position, Vector3 color) :
    _position(position), _color(color) {
}

Vector3 PointLight::evaluateSampleRadiance(Vector3 &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) {
    Vector3 offsetOrigin = surfaceInfo.point() + CADISE_RAY_EPSILON * surfaceInfo.normal();
    Vector3 direction = _position - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();
    pdf = 1.0f;

    return _color;
}

Vector3 PointLight::position() {
    return _position;
}

Vector3 PointLight::color() {
    return _color;
}

} // namespace cadise