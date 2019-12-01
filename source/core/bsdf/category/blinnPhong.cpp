#include "core/bsdf/category/blinnPhong.h"

#include "core/surfaceIntersection.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"

#include <cmath>

namespace cadise {

BlinnPhong::BlinnPhong(const real exponent) :
    Bsdf(BsdfType(BxdfType::GLOSSY_REFLECTION)),
    _exponent(exponent) {
}

Spectrum BlinnPhong::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    const real brdfFactor = math::max(normal.dot(surfaceIntersection.wi()), 0.0_r);

    const Vector3R H = (surfaceIntersection.wi() + surfaceIntersection.wo()).normalize();
    const real NdotH = math::max(normal.dot(H), 0.0_r);
    const real specularValue = std::pow(NdotH, _exponent) * brdfFactor;
	
    return Spectrum(specularValue);
}

Spectrum BlinnPhong::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    const real brdfFactor = math::max(normal.dot(surfaceIntersection.wi()), 0.0_r);

    const Vector3R zAxis(normal);
    Vector3R xAxis;
    Vector3R yAxis;
    math::buildCoordinateSystem(zAxis, &xAxis, &yAxis);

    // TODO : replace glossy sampling as a function
    const real theta = std::acos(std::pow(random::nextReal(), 1.0_r / (_exponent + 1.0_r)));
    const real phi   = constant::TWO_PI * random::nextReal();
    Vector3R sampleH(std::cos(phi) * std::sin(theta),
                     std::sin(phi) * std::sin(theta),
                     std::cos(theta));

    // transform to local coordinate
    sampleH = xAxis * sampleH.x() + yAxis * sampleH.y() + zAxis * sampleH.z();
    sampleH = sampleH.normalize();
    const real NdotH = normal.dot(sampleH);
    const Vector3R outDirection = surfaceIntersection.wi().reflect(sampleH);

    const real pdfH = (_exponent + 1.0_r) * constant::INV_TWO_PI * std::pow(NdotH, _exponent);
    real pdfWo = pdfH / (4.0_r * surfaceIntersection.wi().dot(sampleH));
    pdfWo = math::clamp(pdfWo, 0.0_r, 1.0_r);

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdfWo);

    return Spectrum(std::pow(NdotH, _exponent) * brdfFactor);
}

real BlinnPhong::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();

    const Vector3R H = (surfaceIntersection.wi() + surfaceIntersection.wo()).normalize();
    const real NdotH = math::max(normal.dot(H), 0.0_r);

    const real pdfH = (_exponent + 1.0_r) * constant::INV_TWO_PI * std::pow(NdotH, _exponent);
    real pdfWo = pdfH / (4.0_r * surfaceIntersection.wi().dot(H));
    pdfWo = math::clamp(pdfWo, 0.0_r, 1.0_r);

    return pdfWo;
}

} // namespace cadise