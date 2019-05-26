#include "core/light/areaLight.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

AreaLight::AreaLight(std::shared_ptr<Shape> shape, Vector3F albedo) :
    _shape(shape), _albedo(albedo) {
}

Vector3F AreaLight::evaluateSampleRadiance(Vector3F &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) {
    Vector3F offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.normal();
    SurfaceInfo sampleSurface;
    _shape->sampleSurface(surfaceInfo, sampleSurface);

    Vector3F direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    // Change delta A to delta w
    pdf = _shape->samplePdf();
    pdf *= direction.lengthSquared() / sampleSurface.normal().dot(-direction.normalize());

    return _albedo;
}

Vector3F AreaLight::position() {
    return Vector3F(0.0f, 0.0f, 0.0f);
}

Vector3F AreaLight::color() {
    return _albedo;
}

std::shared_ptr<Shape> AreaLight::shape() {
    return _shape;
}

} // namespace cadise