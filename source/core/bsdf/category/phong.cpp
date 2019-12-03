#include "core/bsdf/category/phong.h"

#include "core/integral-tool/hemisphere.h"
#include "core/surfaceIntersection.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

#include <cmath>

namespace cadise {

Phong::Phong(const real exponent) :
    Bsdf(BsdfType(BxdfType::GLOSSY_REFLECTION)),
    _exponent(exponent) {

    _pdfFactor  = (exponent + 1.0_r) * constant::INV_TWO_PI;
    _brdfFactor = (exponent + 2.0_r) * constant::INV_TWO_PI;
}

Spectrum Phong::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R Ns = surfaceIntersection.surfaceInfo().shadingNormal();
    
    const Vector3R R = surfaceIntersection.wo().reflect(Ns);
    const Vector3R V = surfaceIntersection.wi();
    const real RdotV = math::max(R.dot(V), 0.0_r);
	
    const real specularValue = std::pow(RdotV, _exponent) * _brdfFactor;

    return Spectrum(specularValue);
}

Spectrum Phong::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R Ns = surfaceIntersection.surfaceInfo().shadingNormal();

    // build local coordinate system (shading normal as z-axis)
    const Vector3R zAxis(Ns);
    Vector3R xAxis;
    Vector3R yAxis;
    math::buildCoordinateSystem(zAxis, &xAxis, &yAxis);

    const Vector2R sample(random::nextReal(), random::nextReal());
    Vector3R L;
    Hemisphere::cosineExpWeightedSampling(_exponent, sample, &L);

    // transform L to world coordinate
    L = xAxis * L.x() + yAxis * L.y() + zAxis * L.z();
    L = L.normalize();

    const Vector3R R = L.reflect(Ns);
    const Vector3R V = surfaceIntersection.wi();
    const real RdotV = math::max(R.dot(V), 0.0_r);

    const real powerTerm = std::pow(RdotV, _exponent);
    const real pdfW      = powerTerm * _pdfFactor;

    surfaceIntersection.setWo(L);
    surfaceIntersection.setPdf(pdfW);

    const real specularValue = powerTerm * _brdfFactor;

    return Spectrum(specularValue);
}

real Phong::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R Ns = surfaceIntersection.surfaceInfo().shadingNormal();

    const Vector3R R = surfaceIntersection.wo().reflect(Ns);
    const Vector3R V = surfaceIntersection.wi();
    const real RdotV = math::max(R.dot(V), 0.0_r);

    const real pdfW = std::pow(RdotV, _exponent) * _pdfFactor;

    return pdfW;
}

} // namespace cadise