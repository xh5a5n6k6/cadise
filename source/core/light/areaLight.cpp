#include "core/light/areaLight.h"

#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

AreaLight::AreaLight(std::shared_ptr<Shape> shape, Vector3 albedo) :
    _shape(shape), _albedo(albedo) {
}

Vector3 AreaLight::evaluateSampleRadiance(Vector3 &lightDirection, SurfaceInfo &surfaceInfo, float &t, float &pdf) {
    Vector3 offsetOrigin = surfaceInfo.point() + CADISE_RAY_EPSILON * surfaceInfo.normal();
    SurfaceInfo sampleSurface;
    _shape->sampleSurface(surfaceInfo, sampleSurface);
    Vector3 direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();
    pdf = 1.0f;
    // TODO
    // correct pdf value
    pdf = 1.0f / _shape->area();
    pdf = pdf * direction.lengthSquared() / sampleSurface.normal().dot(-direction.normalize());

    return _albedo;
}

Vector3 AreaLight::position() {
    return Vector3(0.0f, 0.0f, 0.0f);
}

Vector3 AreaLight::color() {
    return _albedo;
}

std::shared_ptr<Shape> AreaLight::shape() {
    return _shape;
}

} // namespace cadise