#include "core/light/areaLight.h"

#include "core/intersector/primitive/primitive.h"
#include "core/surfaceIntersection.h"

#include "math/constant.h"

namespace cadise {

AreaLight::AreaLight(const Spectrum& color, const real watt, const bool isBackFaceEmit) :
    _primitive(),
    _emitRadiance(0.0_r),
    _color(color),
    _watt(watt),
    _isBackFaceEmit(isBackFaceEmit) {
}

Spectrum AreaLight::emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const {
    if (_isBackFaceEmit) {
        return _emitRadiance;
    }

    // check if direction is at the front face 
    const Vector3R frontNormal = emitSurface.frontNormal();
    if (emitDirection.dot(frontNormal) < 0.0_r) {
        return Spectrum(0.0_r);
    }
    else {
        return _emitRadiance;
    }
}

Spectrum AreaLight::evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const {
    const Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.geometryNormal();
    const std::shared_ptr<Primitive> primitive = _primitive.lock();

    SurfaceInfo sampleSurface;
    primitive->sampleSurface(surfaceInfo, sampleSurface);

    const Vector3R direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    const Vector3R frontNormal = sampleSurface.frontNormal();
    if (lightDirection.reverse().dot(frontNormal) < 0.0_r && !_isBackFaceEmit) {
        pdf = 0.0_r;
        return Spectrum(0.0_r);
    }

    // change delta A to delta w
    pdf = primitive->samplePdfA(sampleSurface.point());
    pdf *= direction.lengthSquared() / sampleSurface.geometryNormal().absDot(-direction.normalize());

    return _emitRadiance;
}

real AreaLight::evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const {
    const Vector3R emitPosition  = surfaceIntersection.surfaceInfo().point();
    const Vector3R emitNormal    = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R emitDirection = surfaceIntersection.wi();
    const Vector3R frontNormal   = surfaceIntersection.surfaceInfo().frontNormal();

    if (emitDirection.dot(frontNormal) < 0.0_r && !_isBackFaceEmit) {
        return 0.0_r;
    }

    const std::shared_ptr<Primitive> primitive = _primitive.lock();
    const real pdfA = primitive->samplePdfA(emitPosition);

    return pdfA * distance * distance / emitDirection.absDot(emitNormal);
}

bool AreaLight::isDeltaLight() const {
    return false;
}

void AreaLight::calculateEmitRadiance() {
    const Spectrum unitWattColor = _color / _color.sum();
    const Spectrum totalWattColor = unitWattColor * _watt;

    const std::shared_ptr<Primitive> primitive = _primitive.lock();
    _emitRadiance = totalWattColor / primitive->area() * constant::INV_PI;
}

void AreaLight::setPrimitive(const std::shared_ptr<Primitive>& primitive) {
    _primitive = primitive;
}

void AreaLight::setIsBackFaceEmit(const bool isBackFaceEmit) {
    _isBackFaceEmit = isBackFaceEmit;
}

} // namespace cadise