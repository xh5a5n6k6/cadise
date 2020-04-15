#include "core/surface/bsdf/lambertianDiffuse.h"

#include "core/integral-tool/sample/bsdfSample.h"
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
    LambertianDiffuse(std::make_shared<ConstantTexture<Spectrum>>(Spectrum(0.5_r))) {
}

LambertianDiffuse::LambertianDiffuse(const std::shared_ptr<Texture<Spectrum>>& albedo) :
    Bsdf(BsdfType(BxdfType::DIFFUSE_REFLECTION)),
    _albedo(albedo) {

    CADISE_ASSERT(albedo);
}

Spectrum LambertianDiffuse::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {

    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();
    const Vector3R& L  = surfaceIntersection.wo();

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) {
        return Spectrum(0.0_r);
    }

    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    return sampleSpectrum * constant::inv_pi<real>;
}

void LambertianDiffuse::evaluateSample(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection,
    BsdfSample* const          out_sample) const {

    CADISE_ASSERT(out_sample);

    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();

    // build local coordinate system (shading normal as y-axis)
    const Vector3R yAxis(Ns);
    Vector3R zAxis;
    Vector3R xAxis;
    math::build_coordinate_system(yAxis, &zAxis, &xAxis);

    const Vector2R sample(Random::nextReal(), Random::nextReal());
    Vector3R L;
    real pdfW;
    Hemisphere::cosineWeightedSampling(sample, &L, &pdfW);

    // transform L to world coordinate
    L = xAxis * L.x() + yAxis * L.y() + zAxis * L.z();
    L = L.normalize();
    if (V.dot(Ns) <= 0.0_r) {
        L = L.reverse();
    }

    const Vector3R& uvw = surfaceIntersection.surfaceInfo().uvw();
    Spectrum sampleSpectrum;
    _albedo->evaluate(uvw, &sampleSpectrum);

    out_sample->setScatterValue(sampleSpectrum * constant::inv_pi<real>);
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfW);
}

real LambertianDiffuse::evaluatePdfW(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {

    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();
    const Vector3R& L  = surfaceIntersection.wo();

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) {
        return 0.0_r;
    }

    return L.absDot(Ns) * constant::inv_pi<real>;
}

} // namespace cadise