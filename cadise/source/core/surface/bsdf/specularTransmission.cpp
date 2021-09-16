#include "core/surface/bsdf/specularTransmission.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/integral-tool/tSurfaceSampler.h"
#include "core/surface/fresnel/dielectricFresnel.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

#include <cmath>
#include <utility>

namespace cadise 
{

SpecularTransmission::SpecularTransmission(
    const std::shared_ptr<TTexture<Spectrum>>& albedo,
    const std::shared_ptr<DielectricFresnel>&  fresnel) :
    
    Bsdf(BsdfLobes({ ELobe::SPECULAR_TRANSMISSION })),
    _albedo(albedo),
    _fresnel(fresnel)
{
    CADISE_ASSERT(albedo);
    CADISE_ASSERT(fresnel);
}

Spectrum SpecularTransmission::evaluate(
    const TransportInfo&       info, 
    const SurfaceIntersection& si) const 
{
    return Spectrum(0.0_r);
}

void SpecularTransmission::evaluateSample(
    const TransportInfo&       info, 
    const SurfaceIntersection& si,
    BsdfSample* const          out_sample) const
{
    CADISE_ASSERT(out_sample);

    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();

    real etaI = _fresnel->iorOuter();
    real etaT = _fresnel->iorInner();

    Vector3R L;
    if (!V.canRefract(Ns, etaI, etaT, &L))
    {
        return;
    }

    const real cosThetaI = L.dot(Ns);
    Spectrum reflectance;
    _fresnel->evaluateReflectance(cosThetaI, &reflectance);

    real btdfFactor = 1.0_r;
    if (info.mode() == ETransportMode::RADIANCE)
    {
        if (cosThetaI < 0.0_r) 
        {
            std::swap(etaI, etaT);
        }

        btdfFactor = (etaT * etaT) / (etaI * etaI);
    }
    
    const real     pdfW          = 1.0_r;
    const real     LdotN         = std::abs(cosThetaI);
    const Spectrum transmittance = reflectance.complement();

    Spectrum sampleAlbedo;
    TSurfaceSampler<Spectrum>().sample(si, _albedo.get(), &sampleAlbedo);

    out_sample->setScatterValue(sampleAlbedo.mul(transmittance).mul(btdfFactor / LdotN));
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfW);
}

real SpecularTransmission::evaluatePdfW(
    const TransportInfo&       info, 
    const SurfaceIntersection& si) const
{
    return 0.0_r;
}

ELobe SpecularTransmission::lobe(const BsdfComponents component) const 
{
    CADISE_ASSERT_EQ(component, 0);

    return ELobe::SPECULAR_TRANSMISSION;
}

} // namespace cadise