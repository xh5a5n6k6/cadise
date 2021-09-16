#include "core/surface/bsdf/phongBsdf.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/surfaceIntersection.h"
#include "math/constant.h"
#include "math/random.h"
#include "math/warp/hemisphere.h"

#include <algorithm>
#include <cmath>

namespace cadise 
{

PhongBsdf::PhongBsdf(const real exponent) :
    Bsdf(BsdfLobes({ ELobe::GLOSSY_REFLECTION })),
    _exponent(exponent) 
{
    _pdfFactor  = (exponent + 1.0_r) * constant::rcp_two_pi<real>;
    _brdfFactor = (exponent + 2.0_r) * constant::rcp_two_pi<real>;
}

Spectrum PhongBsdf::evaluate(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const 
{
    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();
    const Vector3R  R  = L.reflect(Ns);

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) 
    {
        return Spectrum(0.0_r);
    }

    const real RdotV         = R.dot(V);
    const real safeRdotV     = std::max(RdotV, 0.0_r);
    const real specularValue = std::pow(safeRdotV, _exponent) * _brdfFactor;

    return Spectrum(specularValue);
}

void PhongBsdf::evaluateSample(
    const TransportInfo&       info,
    const SurfaceIntersection& si,
    BsdfSample* const          out_sample) const
{
    CADISE_ASSERT(out_sample);

    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();

    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Vector3R L;
    real pdfW;
    Hemisphere::cosineExpWeightedSampling(sample, _exponent, &L, &pdfW);

    // transform L to world coordinate
    L = si.surfaceDetail().shadingLcs().localToWorld(L);
    L.normalizeLocal();

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) 
    {
        return;
    }

    const Vector3R R         = L.reflect(Ns);
    const real     RdotV     = R.dot(V);
    const real     safeRdotV = std::max(RdotV, 0.0_r);
    const real     powerTerm = std::pow(safeRdotV, _exponent);

    // TODO: use pdfW instead ?
    const real pdfL          = powerTerm * _pdfFactor;
    const real specularValue = powerTerm * _brdfFactor;

    out_sample->setScatterValue(Spectrum(specularValue));
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfL);
}

real PhongBsdf::evaluatePdfW(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const 
{
    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();
    const Vector3R  R  = L.reflect(Ns);

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r)
    {
        return 0.0_r;
    }

    const real RdotV     = R.dot(V);
    const real safeRdotV = std::max(RdotV, 0.0_r);
    const real pdfL      = std::pow(safeRdotV, _exponent) * _pdfFactor;

    return pdfL;
}

ELobe PhongBsdf::lobe(const BsdfComponents component) const 
{
    CADISE_ASSERT_EQ(component, 0);

    return ELobe::GLOSSY_REFLECTION;
}

} // namespace cadise