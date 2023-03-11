#include "Core/Surface/BSDF/DielectricMicrofacet.h"

#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Surface/Fresnel/DielectricFresnel.h"
#include "Core/Surface/Microfacet/Microfacet.h"
#include "Core/Surface/Microfacet/MicrofacetHelper.h"
#include "Core/Surface/TransportInfo.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"
#include "Math/math.h"
#include "Math/Random.h"

#include <utility>

namespace cadise 
{

DielectricMicrofacet::DielectricMicrofacet(
    const std::shared_ptr<Microfacet>&        microfacet,
    const std::shared_ptr<DielectricFresnel>& fresnel) :

    BSDF(BSDFLobes({ ELobe::GlossyReflection, ELobe::GlossyTransmission })),
    _microfacet(microfacet),
    _fresnel(fresnel) 
{
    CS_ASSERT(microfacet);
    CS_ASSERT(fresnel);
}

Spectrum DielectricMicrofacet::evaluate(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const
{
    bool canReflection = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 0);
    bool canRefraction = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 1);
    if (!canReflection && !canRefraction)
    {
        return Spectrum(0.0_r);
    }

    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);
    if (VdotN * LdotN == 0.0_r) 
    {
        return Spectrum(0.0_r);
    }

    // check scattering event occured at which side (reflection or refraction)
    const bool isReflection = VdotN * LdotN > 0.0_r;
    if (( isReflection && !canReflection) ||
        (!isReflection && !canRefraction)) 
    {
        return Spectrum(0.0_r);
    }

    Vector3R H;
    if (isReflection) 
    {
        if (!MicrofacetHelper::canMakeReflectionH(V, L, Ns, &H)) 
        {
            return Spectrum(0.0_r);
        }
    }
    else 
    {
        if (!MicrofacetHelper::canMakeRefractionH(
            V, L, Ns, _fresnel->iorOuter(), _fresnel->iorInner(), &H))
        {
            return Spectrum(0.0_r);
        }
    }

    const real VdotH = V.dot(H);
    const real LdotH = L.dot(H);
    const real NdotH = Ns.dot(H);

    Spectrum F;
    _fresnel->evaluateReflectance(LdotH, &F);
    if (!isReflection)
    {
        F.complementLocal();
    }

    const real D = _microfacet->distributionD(si, Ns, H);
    const real G = _microfacet->shadowingMaskingG(si, V, L, Ns, H);

    real modelTerm;
    if (isReflection) 
    {
        modelTerm = G * D / (4.0_r * std::abs(VdotN * LdotN));
    }
    else 
    {
        real etaI = _fresnel->iorOuter();
        real etaT = _fresnel->iorInner();
        if (LdotN < 0.0_r) 
        {
            std::swap(etaI, etaT);
        }

        const real btdfFactor = (info.mode() == ETransportMode::Radiance) ? etaT / etaI : 1.0_r;
        const real sqrtTerm   = btdfFactor * etaI / (etaT * VdotH + etaI * LdotH);
        const real mulTerm    = std::abs(VdotH * LdotH / (VdotN * LdotN));

        modelTerm = G * D * mulTerm * sqrtTerm * sqrtTerm;
    }

    return F.mul(modelTerm);
}

void DielectricMicrofacet::evaluateSample(
    const TransportInfo&       info,
    const SurfaceIntersection& si,
    BSDFSample* const          out_sample) const 
{
    CS_ASSERT(out_sample);

    bool canReflection = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 0);
    bool canRefraction = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 1);
    if (!canReflection && !canRefraction)
    {
        return;
    }
    
    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    Vector3R H;
    _microfacet->sampleHalfVectorH(si, sample, &H);

    const Vector3R& Ns    = si.surfaceDetail().shadingNormal();
    const Vector3R& V     = si.wi();
    const real      VdotN = V.dot(Ns);
    const real      VdotH = V.dot(H);
    const real      NdotH = Ns.dot(H);

    Spectrum reflectance;
    _fresnel->evaluateReflectance(VdotH, &reflectance);
    const real reflectionProbability = reflectance.average();

    if (canReflection && canRefraction) 
    {
        const real sampleProbability = Random::nextReal();

        if (sampleProbability < reflectionProbability) 
        {
            canRefraction = false;
        }
        else 
        {
            canReflection = false;
        }
    }

    CS_ASSERT(canReflection || canRefraction);

    real     scatterPdfW = 1.0_r;
    Vector3R scatterDirection(0.0_r);
    Spectrum scatterValue(0.0_r);

    if (canReflection)
    {
        const real      NSign = static_cast<real>(math::sign(VdotN));
        const Vector3R  L     = V.reflect(H.mul(NSign));

        const real LdotN = L.dot(Ns);
        const real LdotH = L.dot(H);
        if (VdotN * LdotN <= 0.0_r)
        {
            return;
        }

        const Spectrum F = reflectance;

        const real D         = _microfacet->distributionD(si, Ns, H);
        const real G         = _microfacet->shadowingMaskingG(si, V, L, Ns, H);
        const real modelTerm = G * D / (4.0_r * std::abs(VdotN * LdotN));

        const real pdfH = std::abs(D * NdotH);
        const real pdfL = std::abs(pdfH / (4.0_r * LdotH));
        if (!std::isfinite(pdfL)) 
        {
            return;
        }

        scatterValue     = F.mul(modelTerm);
        scatterDirection = L;
        scatterPdfW      = pdfL;

        if (info.components() == BSDF_ALL_COMPONENTS)
        {
            scatterPdfW *= reflectionProbability;
        }
    }
    else 
    {
        real etaI = _fresnel->iorOuter();
        real etaT = _fresnel->iorInner();

        Vector3R L;
        if (!V.canRefract(H, etaI, etaT, &L))
        {
            return;
        }

        const real LdotN = L.dot(Ns);
        const real LdotH = L.dot(H);

        real btdfFactor = 1.0_r;
        if (info.mode() == ETransportMode::Radiance) 
        {
            if (LdotH < 0.0_r)
            {
                std::swap(etaI, etaT);
            }

            btdfFactor = (etaT * etaT) / (etaI * etaI);
        }

        Spectrum F;
        _fresnel->evaluateReflectance(LdotH, &F);
        F.complementLocal();

        const real D = _microfacet->distributionD(si, Ns, H);
        const real G = _microfacet->shadowingMaskingG(si, V, L, Ns, H);

        const real sqrtTerm  = etaI / (etaT * VdotH + etaI * LdotH);
        const real mulTerm   = std::abs(VdotH * LdotH / (VdotN * LdotN));
        const real modelTerm = G * D * mulTerm * btdfFactor * sqrtTerm * sqrtTerm;

        const real pdfH = std::abs(D * NdotH);
        const real pdfL = std::abs(pdfH * LdotH * sqrtTerm * sqrtTerm);
        if (!std::isfinite(pdfL))
        {
            return;
        }

        scatterValue     = F.mul(modelTerm);
        scatterDirection = L;
        scatterPdfW      = pdfL;

        if (info.components() == BSDF_ALL_COMPONENTS)
        {
            scatterPdfW *= 1.0_r - reflectionProbability;
        }
    }

    out_sample->setScatterValue(scatterValue);
    out_sample->setScatterDirection(scatterDirection);
    out_sample->setPdfW(scatterPdfW);
}

real DielectricMicrofacet::evaluatePdfW(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const 
{
    bool canReflection = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 0);
    bool canRefraction = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 1);
    if (!canReflection && !canRefraction)
    {
        return 0.0_r;
    }

    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();

    const real VdotN = V.dot(Ns);
    const real LdotN = L.dot(Ns);
    if (VdotN * LdotN == 0.0_r)
    {
        return 0.0_r;
    }

    // check scattering event occured at which side (reflection or refraction)
    const bool isReflection = (VdotN * LdotN > 0.0_r) ? true : false;
    if (( isReflection && !canReflection) ||
        (!isReflection && !canRefraction)) 
    {
        return 0.0_r;
    }

    Vector3R H;
    if (isReflection)
    {
        if (!MicrofacetHelper::canMakeReflectionH(V, L, Ns, &H))
        {
            return 0.0_r;
        }
    }
    else 
    {
        if (!MicrofacetHelper::canMakeRefractionH(
            V, L, Ns, _fresnel->iorOuter(), _fresnel->iorInner(), &H)) 
        {
            return 0.0_r;
        }
    }

    const real VdotH = V.dot(H);
    const real LdotH = L.dot(H);
    const real NdotH = Ns.dot(H);

    Spectrum reflectance;
    _fresnel->evaluateReflectance(LdotH, &reflectance);
    const real reflectionProbability = reflectance.average();

    const real D    = _microfacet->distributionD(si, Ns, H);
    const real pdfH = std::abs(D * NdotH);

    real scatterPdf   = 1.0_r;
    real jacobianTerm = 1.0_r;
    if (isReflection)
    {
        jacobianTerm = std::abs(1.0_r / (4.0_r * LdotH));
        
        if (info.components() == BSDF_ALL_COMPONENTS)
        {
            scatterPdf = reflectionProbability;
        }
    }
    else
    {
        real etaI = _fresnel->iorOuter();
        real etaT = _fresnel->iorInner();
        if (LdotN < 0.0_r) 
        {
            std::swap(etaI, etaT);
        }
        
        const real sqrtTerm = etaI / (etaT * VdotH + etaI * LdotH);

        jacobianTerm = std::abs(LdotH * sqrtTerm * sqrtTerm);

        if (info.components() == BSDF_ALL_COMPONENTS)
        {
            scatterPdf = 1.0_r - reflectionProbability;
        }
    }

    const real pdfL = pdfH * jacobianTerm * scatterPdf;
    if (std::isfinite(pdfL)) 
    {
        return 0.0_r;
    }

    return pdfL;
}

ELobe DielectricMicrofacet::lobe(const BSDFComponents component) const 
{
    CS_ASSERT(component == 0 || component == 1);

    return component == 0 ? ELobe::GlossyReflection : ELobe::GlossyTransmission;
}

} // namespace cadise