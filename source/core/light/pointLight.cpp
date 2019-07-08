#include "core/light/pointLight.h"

#include "core/surfaceGeometryInfo.h"

#include "math/constant.h"

namespace cadise {

PointLight::PointLight(const Vector3R position, const Vector3R color) :
    _position(position), _color(color) {
}

Vector3R PointLight::evaluateSampleRadiance(Vector3R &lightDirection, const SurfaceGeometryInfo surfaceGeometryInfo, real &t, real &pdf) const {
    Vector3R offsetOrigin = surfaceGeometryInfo.point() + constant::RAY_EPSILON * surfaceGeometryInfo.normal();
    Vector3R direction = _position - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();
    pdf = 1.0_r;

    return _color / direction.lengthSquared();
}

Vector3R PointLight::position() const {
    return _position;
}

Vector3R PointLight::color() const {
    return _color;
}

} // namespace cadise