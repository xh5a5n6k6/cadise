#include "core/surface/bsdf/specularReflection.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/surface/fresnel/fresnel.h"
#include "core/surfaceIntersection.h"
#include "core/texture/texture.h"
#include "fundamental/assertion.h"

#include <cmath>

namespace cadise {

SpecularReflection::SpecularReflection(
    const std::shared_ptr<Texture<Spectrum>>& albedo,
    const std::shared_ptr<Fresnel>&           fresnel) :
    
    Bsdf(BsdfLobes({ ELobe::SPECULAR_REFLECTION })),
    _albedo(albedo),
    _fresnel(fresnel) {

    CADISE_ASSERT(albedo);
    CADISE_ASSERT(fresnel);
}

Spectrum SpecularReflection::evaluate(
    const TransportInfo&       info, 
    const SurfaceIntersection& si) const {

    return Spectrum(0.0_r);
}

void SpecularReflection::evaluateSample(
    const TransportInfo&       info, 
    const SurfaceIntersection& si,
    BsdfSample* const          out_sample) const {
    
    CADISE_ASSERT(out_sample);

    const Vector3R& Ns      = si.surfaceInfo().shadingNormal();
    const Vector3R& V       = si.wi();
    const real      VdotN   = V.dot(Ns);
    const real      NFactor = (VdotN > 0.0_r) ? 1.0_r : -1.0_r;
    
    const Vector3R L     = V.reflect(Ns * NFactor);
    const real     LdotN = L.dot(Ns);
    const real     pdfW  = 1.0_r;

    Spectrum reflectance;
    _fresnel->evaluateReflectance(LdotN, &reflectance);

    const Vector3R& uvw = si.surfaceInfo().uvw();
    Spectrum sampleAlbedo;
    _albedo->evaluate(uvw, &sampleAlbedo);

    out_sample->setScatterValue(reflectance * sampleAlbedo / std::abs(LdotN));
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfW);
}

real SpecularReflection::evaluatePdfW(
    const TransportInfo&       info, 
    const SurfaceIntersection& si) const {

    return 0.0_r;
}

} // namespace cadise