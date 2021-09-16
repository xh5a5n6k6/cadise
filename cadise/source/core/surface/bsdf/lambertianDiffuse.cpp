#include "core/surface/bsdf/lambertianDiffuse.h"

#include "core/integral-tool/sample/bsdfSample.h"
#include "core/integral-tool/tSurfaceSampler.h"
#include "core/surfaceIntersection.h"
#include "core/texture/category/tConstantTexture.h"
#include "fundamental/assertion.h"
#include "math/constant.h"
#include "math/math.h"
#include "math/random.h"
#include "math/warp/hemisphere.h"

namespace cadise
{

// Hack
LambertianDiffuse::LambertianDiffuse() :
    LambertianDiffuse(std::make_shared<TConstantTexture<Spectrum>>(Spectrum(0.5_r)))
{}

LambertianDiffuse::LambertianDiffuse(const std::shared_ptr<TTexture<Spectrum>>& albedo) :
    Bsdf(BsdfLobes({ ELobe::DIFFUSE_REFLECTION })),
    _albedo(albedo) 
{
    CADISE_ASSERT(albedo);
}

Spectrum LambertianDiffuse::evaluate(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const 
{
    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();
    
    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) 
    {
        return Spectrum(0.0_r);
    }

    Spectrum sampleAlbedo;
    TSurfaceSampler<Spectrum>().sample(si, _albedo.get(), &sampleAlbedo);

    return sampleAlbedo.mul(constant::rcp_pi<real>);
}

void LambertianDiffuse::evaluateSample(
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
    Hemisphere::cosineWeightedSampling(sample, &L, &pdfW);

    // transform L to world coordinate
    L = si.surfaceDetail().shadingLcs().localToWorld(L);
    L.normalizeLocal();

    if (V.dot(Ns) <= 0.0_r)
    {
        L.negateLocal();
    }

    Spectrum sampleAlbedo;
    TSurfaceSampler<Spectrum>().sample(si, _albedo.get(), &sampleAlbedo);

    out_sample->setScatterValue(sampleAlbedo.mul(constant::rcp_pi<real>));
    out_sample->setScatterDirection(L);
    out_sample->setPdfW(pdfW);
}

real LambertianDiffuse::evaluatePdfW(
    const TransportInfo&       info,
    const SurfaceIntersection& si) const
{
    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();
    const Vector3R& L  = si.wo();

    if (V.dot(Ns) * L.dot(Ns) <= 0.0_r) 
    {
        return 0.0_r;
    }

    return L.absDot(Ns) * constant::rcp_pi<real>;
}

ELobe LambertianDiffuse::lobe(const BsdfComponents component) const 
{
    CADISE_ASSERT_EQ(component, 0);

    return ELobe::DIFFUSE_REFLECTION;
}

} // namespace cadise