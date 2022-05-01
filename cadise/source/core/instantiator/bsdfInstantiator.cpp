#include "core/instantiator/instantiator.h"

// bsdf type
#include "core/surface/bsdf/absorberBsdf.h"
#include "core/surface/bsdf/conductorMicrofacet.h"
#include "core/surface/bsdf/dielectricMicrofacet.h"
#include "core/surface/bsdf/phongBsdf.h"
#include "core/surface/bsdf/lambertianDiffuse.h"
#include "core/surface/bsdf/mixedBsdf.h"
#include "core/surface/bsdf/specularDielectric.h"
#include "core/surface/bsdf/specularReflection.h"
#include "core/surface/bsdf/specularTransmission.h"

#include "core/surface/fresnel/schlickConductorFresnel.h"
#include "core/surface/fresnel/vanillaDielectricFresnel.h"
#include "core/surface/microfacet/anisotropicGgx.h"
#include "core/surface/microfacet/isotropicBeckmann.h"
#include "core/surface/microfacet/isotropicBlinnPhong.h"
#include "core/surface/microfacet/isotropicGgx.h"
#include "core/texture/tTexture.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

#include <iostream>

namespace cadise::instantiator 
{

static std::shared_ptr<Bsdf> createLambertianDiffuse(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures) 
{
    const auto albedo = data->getSpectrumTexture("albedo", spectrumTextures);

    return std::make_shared<LambertianDiffuse>(albedo);
}

static std::shared_ptr<Bsdf> createSpecularReflection(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const auto albedo      = data->getSpectrumTexture("albedo", spectrumTextures);
    const auto fresnelType = data->findString("fresnel-type", "conductor");
    
    std::shared_ptr<Fresnel> fresnel = nullptr;
    if (fresnelType == "conductor") 
    {
        const auto f0 = data->findVector3r("f0", Vector3R(1.0_r));

        fresnel = std::make_shared<SchlickConductorFresnel>(Spectrum(f0));
    }
    else if (fresnelType == "dielectric")
    {
        const real iorOuter = data->findReal("ior-outer", 1.0_r);
        const real iorInner = data->findReal("ior-inner", 1.5_r);

        fresnel = std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner);
    }
    else
    {
        std::cout << "Unknown fresnel type <" << fresnelType << "> in specular reflection"
                  << std::endl;
    }

    return std::make_shared<SpecularReflection>(albedo, fresnel);
}

static std::shared_ptr<Bsdf> createSpecularTransmission(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const auto albedo   = data->getSpectrumTexture("albedo", spectrumTextures);
    const real iorOuter = data->findReal("ior-outer", 1.0_r);
    const real iorInner = data->findReal("ior-inner", 1.5_r);
    
    const auto fresnel = std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner);
    
    return std::make_shared<SpecularTransmission>(albedo, fresnel);
}

static std::shared_ptr<Bsdf> createSpecularDielectric(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures) 
{
    const auto albedo   = data->getSpectrumTexture("albedo", spectrumTextures);
    const real iorOuter = data->findReal("ior-outer", 1.0_r);
    const real iorInner = data->findReal("ior-inner", 1.5_r);

    const auto fresnel = std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner);

    return std::make_shared<SpecularDielectric>(albedo, fresnel);
}

static std::shared_ptr<Bsdf> createPhong(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures) 
{
    const real exponent = data->findReal("exponent", 32.0_r);

    return std::make_shared<PhongBsdf>(exponent);
}

static std::shared_ptr<Bsdf> createPlastic(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures) 
{
    const auto diffuseAlbedo    = data->getSpectrumTexture("diffuse-albedo", spectrumTextures);
    const real specularExponent = data->findReal("specular-exponent", 32.0_r);
    const real diffuseRatio     = data->findReal("diffuse-ratio", 0.7_r);

    return std::make_shared<MixedBsdf>(std::make_shared<LambertianDiffuse>(diffuseAlbedo),
                                       std::make_shared<PhongBsdf>(specularExponent),
                                       diffuseRatio);
}

static std::shared_ptr<Bsdf> createMixed(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures,
    const TStringKeyMap<Bsdf>&               bsdfs) 
{
    const auto   bsdfAName = data->findString("bsdf-a");
    const auto&& bsdfA     = bsdfs.find(bsdfAName);

    CS_ASSERT_NE(bsdfA, bsdfs.end());

    const auto   bsdfBName = data->findString("bsdf-b");
    const auto&& bsdfB     = bsdfs.find(bsdfBName);

    CS_ASSERT_NE(bsdfB, bsdfs.end());

    const real ratio = data->findReal("ratio", 0.5_r);

    return std::make_shared<MixedBsdf>(bsdfA->second,
                                       bsdfB->second,
                                       ratio);
}

static std::shared_ptr<Bsdf> createConductorMicrofacet(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const auto microfacetType = data->findString("microfacet-type", "ggx");
    const auto fresnelType    = data->findString("fresnel-type", "schlick");
    const auto roughness      = data->getRealTexture("roughness", realTextures);
    const auto roughnessU     = data->getRealTexture("roughness-u", realTextures);
    const auto roughnessV     = data->getRealTexture("roughness-v", realTextures);

    std::shared_ptr<Microfacet> microfacet = nullptr;
    if (microfacetType == "beckmann")
    {
        microfacet = std::make_shared<IsotropicBeckmann>(roughness);
    }
    else if (microfacetType == "ggx") 
    {
        microfacet = std::make_shared<IsotropicGgx>(roughness);
    }
    else if (microfacetType == "blinn-phong")
    {
        microfacet = std::make_shared<IsotropicBlinnPhong>(roughness);
    }
    else if (microfacetType == "anisotropic-ggx")
    {
        microfacet = std::make_shared<AnisotropicGgx>(roughnessU, roughnessV);
    }
    else 
    {
        microfacet = std::make_shared<IsotropicGgx>(roughness);
    }

    std::shared_ptr<ConductorFresnel> fresnel = nullptr;
    if (fresnelType == "schlick")
    {
        const auto f0 = data->findVector3r("f0", Vector3R(1.0_r));
        fresnel = std::make_shared<SchlickConductorFresnel>(Spectrum(f0));
    }
    else 
    {
        const auto f0 = data->findVector3r("f0", Vector3R(1.0_r));
        fresnel = std::make_shared<SchlickConductorFresnel>(Spectrum(f0));
    }

    return std::make_shared<ConductorMicrofacet>(microfacet, fresnel);
}

static std::shared_ptr<Bsdf> createDielectricMicrofacet(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const auto microfacetType = data->findString("microfacet-type", "ggx");
    const auto fresnelType    = data->findString("fresnel-type", "vanilla");
    const auto roughness      = data->getRealTexture("roughness", realTextures);
    const auto roughnessU     = data->getRealTexture("roughness-u", realTextures);
    const auto roughnessV     = data->getRealTexture("roughness-v", realTextures);

    std::shared_ptr<Microfacet> microfacet = nullptr;
    if (microfacetType == "beckmann") 
    {
        microfacet = std::make_shared<IsotropicBeckmann>(roughness);
    }
    else if (microfacetType == "ggx") 
    {
        microfacet = std::make_shared<IsotropicGgx>(roughness);
    }
    else if (microfacetType == "blinn-phong")
    {
        microfacet = std::make_shared<IsotropicBlinnPhong>(roughness);
    }
    else if (microfacetType == "anisotropic-ggx")
    {
        microfacet = std::make_shared<AnisotropicGgx>(roughnessU, roughnessV);
    }
    else 
    {
        microfacet = std::make_shared<IsotropicGgx>(roughness);
    }

    std::shared_ptr<DielectricFresnel> fresnel = nullptr;
    if (fresnelType == "vanilla")
    {
        const real iorOuter = data->findReal("ior-outer", 1.0_r);
        const real iorInner = data->findReal("ior-inner", 1.5_r);

        fresnel = std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner);
    }
    else 
    {
        const real iorOuter = data->findReal("ior-outer", 1.0_r);
        const real iorInner = data->findReal("ior-inner", 1.5_r);

        fresnel = std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner);
    }

    return std::make_shared<DielectricMicrofacet>(microfacet, fresnel);
}

std::shared_ptr<Bsdf> makeBsdf(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures,
    const TStringKeyMap<Bsdf>&               bsdfs)
{
    CS_ASSERT(data);

    std::shared_ptr<Bsdf> bsdf = nullptr;

    const auto type = data->findString("type");
    if (type == "matte-lambertian") 
    {
        bsdf = createLambertianDiffuse(data, realTextures, spectrumTextures);
    }
    else if (type == "mirror")
    {
        bsdf = createSpecularReflection(data, realTextures, spectrumTextures);
    }
    else if (type == "transmission") 
    {
        bsdf = createSpecularTransmission(data, realTextures, spectrumTextures);
    }
    else if (type == "glass") 
    {
        bsdf = createSpecularDielectric(data, realTextures, spectrumTextures);
    }
    else if (type == "phong") 
    {
        bsdf = createPhong(data, realTextures, spectrumTextures);
    }
    else if (type == "plastic") 
    {
        bsdf = createPlastic(data, realTextures, spectrumTextures);
    }
    else if (type == "mixed") 
    {
        bsdf = createMixed(data, realTextures, spectrumTextures, bsdfs);
    }
    else if (type == "conductor-microfacet")
    {
        bsdf = createConductorMicrofacet(data, realTextures, spectrumTextures);
    }
    else if (type == "dielectric-microfacet")
    {
        bsdf = createDielectricMicrofacet(data, realTextures, spectrumTextures);
    }
    else if (type == "absorb") 
    {
        bsdf = std::make_shared<AbsorberBsdf>();
    }
    else 
    {
        // unsupported bsdf type
        std::cout << "Unsupported bsdf type: <" << type << ">" << std::endl;
    }

    return bsdf;
}

} // namespace cadise::instantiator