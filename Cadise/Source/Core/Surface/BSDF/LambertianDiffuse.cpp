#include "Core/Surface/BSDF/LambertianDiffuse.h"

#include "Core/Gear/Sample/BSDFSample.h"
#include "Core/Gear/TSurfaceSampler.h"
#include "Core/SurfaceIntersection.h"
#include "Core/Texture/Category/TConstantTexture.h"
#include "Foundation/Assertion.h"
#include "Math/Constant.h"
#include "Math/Random.h"
#include "Math/Warp/Hemisphere.h"

namespace cadise
{

// Hack
LambertianDiffuse::LambertianDiffuse() :
    LambertianDiffuse(std::make_shared<TConstantTexture<Spectrum>>(Spectrum(0.5_r)))
{}

LambertianDiffuse::LambertianDiffuse(const std::shared_ptr<TTexture<Spectrum>>& albedo) :
    BSDF(BSDFLobes({ ELobe::DiffuseReflection })),
    _albedo(albedo)
{
    CS_ASSERT(albedo);
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
    BSDFSample* const          out_sample) const
{
    CS_ASSERT(out_sample);

    const Vector3R& Ns = si.surfaceDetail().shadingNormal();
    const Vector3R& V  = si.wi();

    const std::array<real, 2> sample = { Random::nextReal(), Random::nextReal() };
    real pdfW;
    Vector3R L = Hemisphere::cosineWeightedSampling(sample, &pdfW);

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

ELobe LambertianDiffuse::lobe(const BSDFComponents component) const
{
    CS_ASSERT_EQ(component, 0);

    return ELobe::DiffuseReflection;
}

} // namespace cadise