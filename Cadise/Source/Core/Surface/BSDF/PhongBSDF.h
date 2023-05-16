#pragma once

#include "Core/Surface/BSDF/BSDF.h"

namespace cadise
{

/*
    Implement modified Phong model (only specular term).

    Original Phong model doesn't obey BRDF's rule such as
    helmholtz reciprocity, so we need to add normalization term
    to correct it.

    You can use pre-defind plastic bsdf to assign other
    parameters like diffuse albedo or diffuse-specular ratio.
    (you can see source/Core/instantiator/bsdfInstantiator.cpp for more details.)

    Reference Note:
    http://www.farbrausch.de/~fg/stuff/phong.pdf
    by Fabian Giesen

    http://www.thetenthplanet.de/archives/255
    by christian
*/
class PhongBSDF : public BSDF
{
public:
    explicit PhongBSDF(const real exponent);

    Spectrum evaluate(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const override;

    void evaluateSample(
        const TransportInfo&       info,
        const SurfaceIntersection& si,
        BSDFSample* const          out_sample) const override;

    real evaluatePdfW(
        const TransportInfo&       info,
        const SurfaceIntersection& si) const override;

    ELobe lobe(const BSDFComponents component) const override;

private:
    real _exponent;
    real _pdfFactor;
    real _brdfFactor;
};

} // namespace cadise