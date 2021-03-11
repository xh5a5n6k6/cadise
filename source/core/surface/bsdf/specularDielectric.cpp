#include "core/surface/bsdf/specularDielectric.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/integral-tool/tSurfaceSampler.h"
#include "core/surface/fresnel/dielectricFresnel.h"
#include "core/surface/transportInfo.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/random.h"

#include <cmath>

namespace cadise {

SpecularDielectric::SpecularDielectric(
    const std::shared_ptr<TTexture<Spectrum>>& albedo, 
    const std::shared_ptr<DielectricFresnel>&  fresnel) :
    
    Bsdf(BsdfLobes({ ELobe::SPECULAR_REFLECTION, ELobe::SPECULAR_TRANSMISSION })),
    _albedo(albedo),
    _fresnel(fresnel) {

    CADISE_ASSERT(albedo);
    CADISE_ASSERT(fresnel);
}

Spectrum SpecularDielectric::evaluate(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const {

    return Spectrum(0.0_r);
}

void SpecularDielectric::evaluateSample(
    const TransportInfo&       info,
    const SurfaceIntersection& si,
    BsdfSample* const          out_sample) const {

    CADISE_ASSERT(out_sample);

    const Vector3R& Ns    = si.surfaceDetail().shadingNormal();
    const Vector3R& V     = si.wi();
    const real      VdotN = V.dot(Ns);
    
    Spectrum reflectance;
    _fresnel->evaluateReflectance(VdotN, &reflectance);
    const real reflectionProbability = reflectance.average();
    const real sampleProbability     = Random::nextReal();
    
    bool canReflection = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 0);
    bool canRefraction = (info.components() == BSDF_ALL_COMPONENTS) || (info.components() == 1);

    if (canReflection && canRefraction) {
        if (sampleProbability < reflectionProbability) {
            canRefraction = false;
        }
        else {
            canReflection = false;
        }
    }

    real     scatterPdfW = 1.0_r;
    Vector3R scatterDirection(0.0_r);
    Spectrum scatterValue(0.0_r);

    if (canReflection) {
        const real     NFactor = (VdotN > 0.0_r) ? 1.0_r : -1.0_r;
        const Vector3R L       = V.reflect(Ns * NFactor);
        const real     LdotN   = L.absDot(Ns);

        Spectrum sampleAlbedo;
        TSurfaceSampler<Spectrum>().sample(si, _albedo.get(), &sampleAlbedo);

        scatterValue     = sampleAlbedo * reflectance / LdotN;
        scatterDirection = L;

        if (info.components() == BSDF_ALL_COMPONENTS) {
            scatterPdfW = reflectionProbability;
        }
    }
    else if (canRefraction) {
        real etaI = _fresnel->iorOuter();
        real etaT = _fresnel->iorInner();
        
        Vector3R L;
        if (!V.canRefract(Ns, etaI, etaT, &L)) {
            return;
        }
    
        const real cosThetaI = L.dot(Ns);
        Spectrum refractDirectionReflectance;
        _fresnel->evaluateReflectance(cosThetaI, &refractDirectionReflectance);

        real btdfFactor = 1.0_r;
        if (info.mode() == ETransportMode::RADIANCE) {
            if (cosThetaI < 0.0_r) {
                math::swap(etaI, etaT);
            }

            btdfFactor = (etaT * etaT) / (etaI * etaI);
        }

        const real     LdotN         = std::abs(cosThetaI);
        const Spectrum transmittance = refractDirectionReflectance.complement();

        Spectrum sampleAlbedo;
        TSurfaceSampler<Spectrum>().sample(si, _albedo.get(), &sampleAlbedo);

        scatterValue     = sampleAlbedo * transmittance * btdfFactor / LdotN;
        scatterDirection = L;

        if (info.components() == BSDF_ALL_COMPONENTS) {
            scatterPdfW = 1.0_r - reflectionProbability;
        }
    }
    else {
        // something wrong
        CADISE_ASSERT(false);
    }

    out_sample->setScatterValue(scatterValue);
    out_sample->setScatterDirection(scatterDirection);
    out_sample->setPdfW(scatterPdfW);
}

real SpecularDielectric::evaluatePdfW(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const {

    return 0.0_r;
}

ELobe SpecularDielectric::lobe(const BsdfComponents component) const {
    CADISE_ASSERT(component == 0 || component == 1);

    return component == 0 ? ELobe::SPECULAR_REFLECTION : ELobe::SPECULAR_TRANSMISSION;
}

} // namespace cadise