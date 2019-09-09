#include "core/light/areaLight.h"

#include "core/intersector/primitive/primitive.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

namespace cadise {

AreaLight::AreaLight(const Spectrum& albedo, const bool isBackFaceEmit) :
    _albedo(albedo),
    _primitive(),
    _isBackFaceEmit(isBackFaceEmit) {
}

Spectrum AreaLight::emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const {
    if (_isBackFaceEmit) {
        return _albedo;
    }

    // check if direction is at the front face 
    const Vector3R frontNormal = emitSurface.frontNormal();
    if (emitDirection.dot(frontNormal) < 0.0_r) {
        return Spectrum(0.0_r);
    }
    else {
        return _albedo;
    }
}

Spectrum AreaLight::evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const {
    Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.geometryNormal();
    SurfaceInfo sampleSurface;
    std::shared_ptr<Primitive> primitive = _primitive.lock();
    primitive->sampleSurface(surfaceInfo, sampleSurface);

    Vector3R direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    Vector3R frontNormal = sampleSurface.frontNormal();
    if (lightDirection.composite().dot(frontNormal) < 0.0_r && !_isBackFaceEmit) {
        pdf = 0.0_r;
        return Spectrum(0.0_r);
    }

    // change delta A to delta w
    pdf = primitive->samplePdfA(sampleSurface.point());
    pdf *= direction.lengthSquared() / sampleSurface.geometryNormal().absDot(-direction.normalize());

    return _albedo;
}

real AreaLight::evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const {
    const Vector3R emitPosition  = surfaceIntersection.surfaceInfo().point();
    const Vector3R emitNormal    = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R emitDirection = surfaceIntersection.wi();
    const Vector3R frontNormal   = surfaceIntersection.surfaceInfo().frontNormal();

    if (emitDirection.dot(frontNormal) < 0.0_r && !_isBackFaceEmit) {
        return 0.0_r;
    }

    std::shared_ptr<Primitive> primitive = _primitive.lock();
    real pdfA = primitive->samplePdfA(emitPosition);

    return pdfA * distance * distance / emitDirection.absDot(emitNormal);
}

bool AreaLight::isDeltaLight() const {
    return false;
}

void AreaLight::setPrimitive(const std::shared_ptr<Primitive>& primitive) {
    _primitive = primitive;
}

} // namespace cadise