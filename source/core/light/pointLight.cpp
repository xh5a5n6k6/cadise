#include "core/light/pointLight.h"

#include "core/surfaceInfo.h"
#include "math/constant.h"

namespace cadise {

PointLight::PointLight(const Vector3R& position, const Spectrum& color) :
    _position(position), 
    _color(color) {
}

Spectrum PointLight::emittance(const Vector3R& emitDirection, const SurfaceIntersection& emitSurface) const {
    return Spectrum(0.0_r);
}

Spectrum PointLight::evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const {
    const Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.geometryNormal();
    const Vector3R direction = _position - offsetOrigin;
    
    t = direction.length();
    lightDirection = direction.normalize();
    pdf = 1.0_r;

    return _color / direction.lengthSquared();
}

real PointLight::evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const {
    return 0.0_r;
}

bool PointLight::isDeltaLight() const {
    return true;
}

} // namespace cadise