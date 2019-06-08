#include "core/light/pointLight.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

PointLight::PointLight(Vector3R position, Vector3R color) :
    _position(position), _color(color) {
}

Vector3R PointLight::evaluateSampleRadiance(Vector3R &lightDirection, SurfaceInfo &surfaceInfo, real &t, real &pdf) {
    Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.normal();
    Vector3R direction = _position - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();
    pdf = 1.0_r;

    return _color;
}

Vector3R PointLight::position() {
    return _position;
}

Vector3R PointLight::color() {
    return _color;
}

} // namespace cadise