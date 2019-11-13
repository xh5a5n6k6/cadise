#include "core/light/singleAreaLight.h"

#include "core/intersector/primitive/primitive.h"
#include "core/surfaceIntersection.h"
#include "core/texture/category/constantTexture.h"

#include "fundamental/assertion.h"

#include "math/constant.h"

namespace cadise {

SingleAreaLight::SingleAreaLight(const Primitive* const primitive,
                                 const Spectrum& color,
                                 const real watt,
                                 const bool isBackFaceEmit) :
    AreaLight(isBackFaceEmit),
    _primitive(primitive) {

    CADISE_ASSERT(primitive);

    const Spectrum unitWattColor = color / color.sum();
    const Spectrum totalWattColor = unitWattColor * watt;

    _emitRadiance = std::make_shared<ConstantTexture<Spectrum>>(
                        totalWattColor / primitive->area() * constant::INV_PI);
}

Spectrum SingleAreaLight::emittance(const Vector3R& emitDirection, const SurfaceInfo& emitSurface) const {
    // check if direction is at the front face 
    const Vector3R frontNormal = emitSurface.frontNormal();
    if (emitDirection.dot(frontNormal) < 0.0_r && !_isBackFaceEmit) {
        return Spectrum(0.0_r);
    }
    else {
        // TODO: fix here
        const Vector3R uvw;
        Spectrum sampleRadiance;
        _emitRadiance->evaluate(uvw, &sampleRadiance);

        return sampleRadiance;
    }
}

Spectrum SingleAreaLight::evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceInfo& surfaceInfo, real& t, real& pdf) const {
    const Vector3R offsetOrigin = surfaceInfo.point() + constant::RAY_EPSILON * surfaceInfo.geometryNormal();

    SurfaceInfo sampleSurface;
    _primitive->sampleSurface(surfaceInfo, sampleSurface);

    const Vector3R direction = sampleSurface.point() - offsetOrigin;
    t = direction.length();
    lightDirection = direction.normalize();

    const Vector3R frontNormal = sampleSurface.frontNormal();
    if (lightDirection.reverse().dot(frontNormal) < 0.0_r && !_isBackFaceEmit) {
        pdf = 0.0_r;
        return Spectrum(0.0_r);
    }

    // change delta A to delta w
    pdf = _primitive->samplePdfA(sampleSurface.point());
    pdf *= direction.lengthSquared() / sampleSurface.geometryNormal().absDot(-direction.normalize());

    // TODO: fix here
    const Vector3R uvw;
    Spectrum sampleRadiance;
    _emitRadiance->evaluate(uvw, &sampleRadiance);

    return sampleRadiance;
}

real SingleAreaLight::evaluatePdfW(const SurfaceIntersection& surfaceIntersection, const real distance) const {
    const Vector3R emitPosition = surfaceIntersection.surfaceInfo().point();
    const Vector3R emitNormal = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R emitDirection = surfaceIntersection.wi();
    const Vector3R frontNormal = surfaceIntersection.surfaceInfo().frontNormal();

    if (emitDirection.dot(frontNormal) < 0.0_r && !_isBackFaceEmit) {
        return 0.0_r;
    }

    const real pdfA = _primitive->samplePdfA(emitPosition);

    return pdfA * distance * distance / emitDirection.absDot(emitNormal);
}

void SingleAreaLight::setEmitRadiance(const std::shared_ptr<Texture<Spectrum>>& emitRadiance) {
    CADISE_ASSERT(emitRadiance);

    _emitRadiance = emitRadiance;
}

} // namespace cadise