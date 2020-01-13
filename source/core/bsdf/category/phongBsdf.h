#pragma once

#include "core/bsdf/bsdf.h"

namespace cadise {

/*
    Implement modified Phong model (only specular term).

    You can use pre-defind plastic bsdf to assign other
    parameters like diffuse albedo or diffuse-specular ratio.
    (you can see source/core/instantiator/bsdfInstantiator.cpp for more details.)

    Original Phong model doesn't obey BRDF's rule such as
    helmholtz reciprocity, so we need to add normalization term
    to correct it.

    Reference Note:
    http://www.farbrausch.de/~fg/stuff/phong.pdf
    by Fabian Giesen

    http://www.thetenthplanet.de/archives/255
    by christian
*/
class PhongBsdf : public Bsdf {
public:
    PhongBsdf(const real exponent);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const override;

private:
    real _exponent;
    real _pdfFactor;
    real _brdfFactor;
};

} // namespace cadise