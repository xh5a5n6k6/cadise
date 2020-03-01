#include "core/bsdf/category/lambertianDiffuse.h"

#include "core/surfaceIntersection.h"
#include "core/texture/category/constantTexture.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"
#include "math/warp/hemisphere.h"

namespace cadise {

// Hack
LambertianDiffuse::LambertianDiffuse() :
    LambertianDiffuse(std::make_shared<ConstantTexture<Spectrum>>(0.5_r)) {
}

LambertianDiffuse::LambertianDiffuse(const std::shared_ptr<Texture<Spectrum>>& albedo) :
    Bsdf(BsdfType(BxdfType::DIFFUSE_REFLECTION)),
    _albedo(albedo) {

    CADISE_ASSERT(albedo);
}

Spectrum LambertianDiffuse::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();
    const Vector3R& L  = surfaceIntersection.wo();

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) {
        return Spectrum(0.0_r);
    }

    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * constant::INV_PI;
}

Spectrum LambertianDiffuse::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();

    // build local coordinate system (shading normal as z-axis)
    const Vector3R zAxis(Ns);
    Vector3R xAxis;
    Vector3R yAxis;
    math::buildCoordinateSystem(zAxis, &xAxis, &yAxis);

    const Vector2R sample(Random::nextReal(), Random::nextReal());
    Vector3R L;
    Hemisphere::cosineWeightedSampling(sample, &L);

    // transform L to world coordinate
    L = xAxis * L.x() + yAxis * L.y() + zAxis * L.z();
    L = L.normalize();
    if (V.dot(Ns) <= 0.0_r) {
        L = L.reverse();
    }

    const real pdfW = L.absDot(zAxis) * constant::INV_PI;

    surfaceIntersection.setWo(L);
    surfaceIntersection.setPdf(pdfW);

    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * constant::INV_PI;
}

real LambertianDiffuse::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();
    const Vector3R& L  = surfaceIntersection.wo();

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) {
        return 0.0_r;
    }

    return L.absDot(Ns) * constant::INV_PI;
}

} // namespace cadise