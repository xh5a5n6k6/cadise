#pragma once

#include "core/bsdf/bsdf.h"

#include <memory>

namespace cadise {

/*
    Original Phong model doesn't obey BRDF's rule such as
    helmholtz reciprocity, so we need to add normalization term
    to correct it.

    Reference Note:
    http://www.farbrausch.de/~fg/stuff/phong.pdf
    by Fabian Giesen

    http://www.thetenthplanet.de/archives/255
    by christian
*/
class Phong : public Bsdf {
public:
    Phong(const real exponent);

    Spectrum evaluate(const SurfaceIntersection& surfaceIntersection) const override;
    Spectrum evaluateSample(SurfaceIntersection& surfaceIntersection) const override;

    real evaluatePdfW(const SurfaceIntersection& surfaceIntersection) const override;

private:
    real _exponent;
    real _pdfFactor;
    real _brdfFactor;
};

} // namespace cadise