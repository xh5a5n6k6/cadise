#include "Core/Surface/BSDF/ConductorMicrofacet.h"

#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Surface/Fresnel/ConductorFresnel.h"
#include "Core/Surface/Microfacet/Microfacet.h"
#include "Core/Surface/Microfacet/MicrofacetUtility.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Math/Random.h"

#include <cmath>

namespace cadise
{

ConductorMicrofacet::ConductorMicrofacet(
    const std::shared_ptr<Microfacet>&       microfacet,
    const std::shared_ptr<ConductorFresnel>& fresnel) :

    BSDF(BSDFLobes({ ELobe::GlossyReflection })),
    _microfacet(microfacet),
    _fresnel(fresnel)
{
    CS_ASSERT(microfacet);
    CS_ASSERT(fresnel);
}

Spectrum ConductorMicrofacet::evaluate(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const
{
    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);
    if (VdotN * LdotN <= 0.0_r)
    {
        return Spectrum(0.0_r);
    }

    Vector3R H;
    if (!MicrofacetUtility::canMakeReflectionH(V, L, Ns, &H))
    {
        return Spectrum(0.0_r);
    }

    const real LdotH = L.dot(H);
    const real NdotH = Ns.dot(H);

    Spectrum F;
    _fresnel->evaluateReflectance(LdotH, &F);

    const real D         = _microfacet->distributionD(si, Ns, H);
    const real G         = _microfacet->shadowingMaskingG(si, V, L, Ns, H);
    const real modelTerm = G * D / (4.0_r * std::abs(VdotN * LdotN));

    return F.mul(modelTerm);
}

void ConductorMicrofacet::evaluateSample(
    const TransportInfo&       info,
    const SurfaceIntersection& si,
    BSDFSample* const          out_sample) const
{
    CS_ASSERT(out_sample);

    const Vector3R& Ns    = si.surfaceDetail().shadingNormal();
    const Vector3R& V     = si.wi();
    const real      VdotN = V.dot(Ns);

    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Vector3R H;
    _microfacet->sampleHalfVectorH(si, sample, &H);

    const real     NSign = static_cast<real>(math::sign(VdotN));
    const Vector3R L     = V.reflect(H.mul(NSign));
    const real     LdotN = L.dot(Ns);
    if (VdotN * LdotN <= 0.0_r)
    {
        return;
    }

    const real LdotH = L.dot(H);
    const real NdotH = Ns.dot(H);

    Spectrum F;
    _fresnel->evaluateReflectance(LdotH, &F);

    const real D         = _microfacet->distributionD(si, Ns, H);
    const real G         = _microfacet->shadowingMaskingG(si, V, L, Ns, H);
    const real modelTerm = G * D / (4.0_r * std::abs(VdotN * LdotN));

    const real pdfH = std::abs(D * NdotH);
    const real pdfL = std::abs(pdfH / (4.0_r * LdotH));
    if (!std::isfinite(pdfL))
    {
        return;
    }

    out_sample->setScatterValue(F.mul(modelTerm));
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfL);
}

real ConductorMicrofacet::evaluatePdfW(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const
{
    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);
    if (VdotN * LdotN <= 0.0_r)
    {
        return 0.0_r;
    }

    Vector3R H;
    if (!MicrofacetUtility::canMakeReflectionH(V, L, Ns, &H))
    {
        return 0.0_r;
    }

    const real D     = _microfacet->distributionD(si, Ns, H);
    const real NdotH = Ns.dot(H);
    const real LdotH = L.dot(H);

    const real pdfH = std::abs(D * NdotH);
    const real pdfL = std::abs(pdfH / (4.0_r * LdotH));
    if (!std::isfinite(pdfL))
    {
        return 0.0_r;
    }

    return pdfL;
}

ELobe ConductorMicrofacet::lobe(const BSDFComponents component) const
{
    CS_ASSERT_EQ(component, 0);

    return ELobe::GlossyReflection;
}

} // namespace cadise