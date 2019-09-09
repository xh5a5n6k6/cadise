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
    real brdfFactor = math::max(normal.dot(surfaceIntersection.wi()), 0.0_r);

    // H : half vector
    Vector3R H = (surfaceIntersection.wi() + surfaceIntersection.wo()).normalize();
    real NdotH = math::max(normal.dot(H), 0.0_r);
    real specular = std::pow(NdotH, _exponent) * brdfFactor;
	
    return Spectrum(specular);
}

Spectrum BlinnPhong::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    real brdfFactor = math::max(normal.dot(surfaceIntersection.wi()), 0.0_r);

    Vector3R xAxis;
    Vector3R yAxis;
    Vector3R zAxis(normal);
    math::buildCoordinateSystem(zAxis, xAxis, yAxis);

    // TODO : replace glossy sampling as a function
    real theta = std::acos(std::pow(random::nextReal(), 1.0_r / (_exponent + 1.0_r)));
    real phi   = constant::TWO_PI * random::nextReal();
    Vector3R sampleH = Vector3R(std::cos(phi) * std::sin(theta),
                                std::sin(phi) * std::sin(theta),
                                std::cos(theta));

    // transform to local coordinate
    sampleH = xAxis * sampleH.x() + yAxis * sampleH.y() + zAxis * sampleH.z();
    sampleH = sampleH.normalize();
    real NdotH = normal.dot(sampleH);
    Vector3R outDirection = surfaceIntersection.wi().reflect(sampleH);

    real pdfH = (_exponent + 1.0_r) * constant::INV_TWO_PI * std::pow(NdotH, _exponent);
    real pdfWo = pdfH / (4.0_r * surfaceIntersection.wi().dot(sampleH));
    pdfWo = math::clamp(pdfWo, 0.0_r, 1.0_r);

    surfaceIntersection.setWo(outDirection);
    surfaceIntersection.setPdf(pdfWo);

    return Spectrum(std::pow(NdotH, _exponent) * brdfFactor);
}

real BlinnPhong::evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const {
    const Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();

    // H : half vector
    Vector3R H = (surfaceIntersection.wi() + surfaceIntersection.wo()).normalize();
    real NdotH = math::max(normal.dot(H), 0.0_r);

    real pdfH = (_exponent + 1.0_r) * constant::INV_TWO_PI * std::pow(NdotH, _exponent);
    real pdfWo = pdfH / (4.0_r * surfaceIntersection.wi().dot(H));
    pdfWo = math::clamp(pdfWo, 0.0_r, 1.0_r);

    return pdfWo;
}

} // namespace cadise