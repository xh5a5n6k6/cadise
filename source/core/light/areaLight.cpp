#include "core/light/areaLight.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

AreaLight::AreaLight(const std::shared_ptr<Shape> shape, const Vector3R albedo) :
    _shape(shape), _albedo(albedo) {
}

Vector3R AreaLight::evaluateSampleRadiance(Vector3R &lightDirection, const SurfaceInfo surfaceInfo, real &t, real &pdf) const {
    Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.normal();
    SurfaceInfo sampleSurface;
    _shape->sampleSurface(surfaceInfo, sampleSurface);

    Vector3R direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    // Change delta A to delta w
    pdf = _shape->samplePdfA();
    pdf *= direction.lengthSquared() / sampleSurface.normal().dot(-direction.normalize());

    return _albedo;
}

Vector3R AreaLight::position() const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

Vector3R AreaLight::color() const {
    return _albedo;
}

std::shared_ptr<Shape> AreaLight::shape() const {
    return _shape;
}

} // namespace cadise