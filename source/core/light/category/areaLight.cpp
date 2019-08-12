#include "core/light/category/areaLight.h"

#include "core/intersector/primitive/primitive.h"
#include "core/surfaceInfo.h"

#include "math/constant.h"

namespace cadise {

AreaLight::AreaLight(const Spectrum& albedo) :
    _albedo(albedo),
    _primitive() {
}

Spectrum AreaLight::evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const {
    Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.geometryNormal();
    SurfaceInfo sampleSurface;
    std::shared_ptr<Primitive> primitive = _primitive.lock();
    primitive->sampleSurface(surfaceInfo, sampleSurface);

    Vector3R direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    // change delta A to delta w
    pdf = primitive->samplePdfA(sampleSurface.point());
    pdf *= direction.lengthSquared() / sampleSurface.geometryNormal().dot(-direction.normalize());

    return _albedo;
}

Spectrum AreaLight::color() const {
    return _albedo;
}

void AreaLight::setPrimitive(const std::shared_ptr<Primitive>& primitive) {
    _primitive = primitive;
}

} // namespace cadise