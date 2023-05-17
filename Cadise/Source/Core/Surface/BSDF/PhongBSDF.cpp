#include "Core/Surface/BSDF/PhongBSDF.h"

#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/SurfaceIntersection.h"
#include "Math/Constant.h"
#include "Math/Random.h"
#include "Math/Warp/Hemisphere.h"

#include <algorithm>
#include <cmath>

namespace cadise
{

PhongBSDF::PhongBSDF(const real exponent) :
    BSDF(BSDFLobes({ ELobe::GlossyReflection })),
    _exponent(exponent)
{
    _pdfFactor  = (exponent + 1.0_r) * constant::rcp_two_pi<real>;
    _brdfFactor = (exponent + 2.0_r) * constant::rcp_two_pi<real>;
}

Spectrum PhongBSDF::evaluate(
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

void PhongBSDF::evaluateSample(
    const TransportInfo&       info,
    const SurfaceIntersection& si,
    BSDFSample* const          out_sample) const
{
    CS_ASSERT(out_sample);

    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();

    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    real pdfW;
    Vector3R L = Hemisphere::cosineExpWeightedSampling(sample, _exponent, &pdfW);

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

real PhongBSDF::evaluatePdfW(
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

ELobe PhongBSDF::lobe(const BSDFComponents component) const
{
    CS_ASSERT_EQ(component, 0);

    return ELobe::GlossyReflection;
}

} // namespace cadise