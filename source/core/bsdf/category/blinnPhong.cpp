#include "core/bsdf/category/blinnPhong.h"

#include "core/surfaceIntersection.h"

#include <algorithm>
#include <cmath>

namespace cadise {

BlinnPhong::BlinnPhong(const real exponent) :
    Bsdf(BsdfType(BxdfType::GLOSSY_REFLECTION)),
    _exponent(exponent) {
}

Spectrum BlinnPhong::evaluate(const SurfaceIntersection& surfaceIntersection) const {
    Vector3R normal = surfaceIntersection.surfaceInfo().shadingNormal();
    real brdfFactor = std::max(normal.dot(surfaceIntersection.wi()), 0.0_r);

    // H : half vector
    Vector3R H = (surfaceIntersection.wi() + surfaceIntersection.wo()).normalize();
    real NdotH = std::max(H.dot(normal), 0.0_r);
    real specular = std::pow(NdotH, _exponent) * brdfFactor;
	
    return Spectrum(specular);
}

Spectrum BlinnPhong::evaluateSample(SurfaceIntersection& surfaceIntersection) const {
    return Spectrum(0.0_r);
}

} // namespace cadise