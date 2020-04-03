#include "core/surface/bsdf/phongBsdf.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/surfaceIntersection.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"
#include "math/warp/hemisphere.h"

#include <cmath>

namespace cadise {

PhongBsdf::PhongBsdf(const real exponent) :
    Bsdf(BsdfType(BxdfType::GLOSSY_REFLECTION)),
    _exponent(exponent) {

    _pdfFactor  = (exponent + 1.0_r) * constant::inv_two_pi<real>;
    _brdfFactor = (exponent + 2.0_r) * constant::inv_two_pi<real>;
}

Spectrum PhongBsdf::evaluate(
    const TransportInfo&       transportInfo,
    const SurfaceIntersection& surfaceIntersection) const {

    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();
    const Vector3R& L  = surfaceIntersection.wo();
    const Vector3R  R  = L.reflect(Ns);

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) {
        return Spectrum(0.0_r);
    }

    const real RdotV         = math::max(R.dot(V), 0.0_r);
    const real specularValue = std::pow(RdotV, _exponent) * _brdfFactor;

    return Spectrum(specularValue);
}

void PhongBsdf::evaluateSample(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection,
    BsdfSample* const          out_sample) const {

    CADISE_ASSERT(out_sample);

    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();

    // build local coordinate system (shading normal as z-axis)
    const Vector3R zAxis(Ns);
    Vector3R xAxis;
    Vector3R yAxis;
    math::buildCoordinateSystem(zAxis, &xAxis, &yAxis);

    const Vector2R sample(Random::nextReal(), Random::nextReal());
    Vector3R L;
    real pdfW;
    Hemisphere::cosineExpWeightedSampling(sample, _exponent, &L, &pdfW);

    // transform L to world coordinate
    L = xAxis * L.x() + yAxis * L.y() + zAxis * L.z();
    L = L.normalize();

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) {
        return;
    }

    const Vector3R R         = L.reflect(Ns);
    const real     RdotV     = math::max(R.dot(V), 0.0_r);
    const real     powerTerm = std::pow(RdotV, _exponent);

    // TODO: use pdfW instead ?
    const real pdfL          = powerTerm * _pdfFactor;
    const real specularValue = powerTerm * _brdfFactor;

    out_sample->setScatterValue(Spectrum(specularValue));
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfL);
}

real PhongBsdf::evaluatePdfW(
    const TransportInfo&       transportInfo, 
    const SurfaceIntersection& surfaceIntersection) const {

    const Vector3R& Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    const Vector3R& V  = surfaceIntersection.wi();
    const Vector3R& L  = surfaceIntersection.wo();
    const Vector3R  R  = L.reflect(Ns);

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) {
        return 0.0_r;
    }

    const real RdotV = math::max(R.dot(V), 0.0_r);
    const real pdfL  = std::pow(RdotV, _exponent) * _pdfFactor;

    return pdfL;
}

} // namespace cadise