#include "core/light/areaLight.h"

#include "core/intersector/primitive/primitive.h"
#include "core/surfaceGeometryInfo.h"

#include "math/constant.h"

namespace cadise {

AreaLight::AreaLight(const Vector3R& albedo) :
    _albedo(albedo) {
}

Vector3R AreaLight::evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceGeometryInfo& surfaceGeometryInfo, real& t, real& pdf) const {
    Vector3R offsetOrigin = surfaceGeometryInfo.point() + constant::RAY_EPSILON * surfaceGeometryInfo.normal();
    SurfaceGeometryInfo sampleSurface;
    std::shared_ptr<Primitive> primitive = _primitive.lock();
    primitive->sampleSurface(surfaceGeometryInfo, sampleSurface);

    Vector3R direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    // change delta A to delta w
    pdf = primitive->samplePdfA(sampleSurface.point());
    pdf *= direction.lengthSquared() / sampleSurface.normal().dot(-direction.normalize());

    return _albedo;
}

Vector3R AreaLight::position() const {
    return Vector3R(0.0_r, 0.0_r, 0.0_r);
}

Vector3R AreaLight::color() const {
    return _albedo;
}

void AreaLight::setPrimitive(const std::shared_ptr<Primitive>& primitive) {
    _primitive = primitive;
}

} // namespace cadise