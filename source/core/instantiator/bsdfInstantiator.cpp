#include "core/instantiator/instantiator.h"

// bsdf type
#include "core/surface/bsdf/absorberBsdf.h"
#include "core/surface/bsdf/conductorMicrofacet.h"
#include "core/surface/bsdf/phongBsdf.h"
#include "core/surface/bsdf/lambertianDiffuse.h"
#include "core/surface/bsdf/mixedBsdf.h"
#include "core/surface/bsdf/specularDielectric.h"
#include "core/surface/bsdf/specularReflection.h"
#include "core/surface/bsdf/specularTransmission.h"

#include "core/surface/fresnel/schlickConductorFresnel.h"
#include "core/surface/fresnel/vanillaDielectricFresnel.h"
#include "core/surface/microfacet/isotropicBeckmann.h"
#include "core/surface/microfacet/isotropicBlinnPhong.h"
#include "core/surface/microfacet/isotropicTrowbridgeReitz.h"
#include "core/texture/texture.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

#include <iostream>

namespace cadise {

namespace instantiator {

static std::shared_ptr<Bsdf> createLambertianDiffuse(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo = data->getSpectrumTexture("albedo", spectrumTextures);

    return std::make_shared<LambertianDiffuse>(albedo);
}

static std::shared_ptr<Bsdf> createSpecularReflection(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo = data->getSpectrumTexture("albedo", spectrumTextures);
    const real                               iorOuter = data->findReal("ior-outer", 1.0_r);
    const real                               iorInner = data->findReal("ior-inner", 1.0_r);

    return std::make_shared<SpecularReflection>(albedo,
                                                std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner));
}

static std::shared_ptr<Bsdf> createSpecularTransmission(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo   = data->getSpectrumTexture("albedo", spectrumTextures);
    const real                               iorOuter = data->findReal("ior-outer", 1.0_r);
    const real                               iorInner = data->findReal("ior-inner", 1.0_r);

    return std::make_shared<SpecularTransmission>(albedo,
                                                  std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner));
}

static std::shared_ptr<Bsdf> createSpecularDielectric(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo   = data->getSpectrumTexture("albedo", spectrumTextures);
    const real                               iorOuter = data->findReal("ior-outer", 1.0_r);
    const real                               iorInner = data->findReal("ior-inner", 1.5_r);

    return std::make_shared<SpecularDielectric>(albedo,
                                                std::make_shared<VanillaDielectricFresnel>(iorOuter, iorInner));
}

static std::shared_ptr<Bsdf> createPhong(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const real exponent = data->findReal("exponent", 32.0_r);

    return std::make_shared<PhongBsdf>(exponent);
}

static std::shared_ptr<Bsdf> createPlastic(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> diffuseAlbedo
        = data->getSpectrumTexture("diffuse-albedo", spectrumTextures);
    const real specularExponent = data->findReal("specular-exponent", 32.0_r);
    const real diffuseRatio     = data->findReal("diffuse-ratio", 0.7_r);

    return std::make_shared<MixedBsdf>(std::make_shared<LambertianDiffuse>(diffuseAlbedo),
                                       std::make_shared<PhongBsdf>(specularExponent),
                                       diffuseRatio);
}

static std::shared_ptr<Bsdf> createMixed(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures,
    const StringKeyMap<Bsdf>& bsdfs) {

    const std::string_view bsdfAName = data->findString("bsdf-a");
    const auto&& bsdfA = bsdfs.find(bsdfAName);

    CADISE_ASSERT_NE(bsdfA, bsdfs.end());

    const std::string_view bsdfBName = data->findString("bsdf-b");
    const auto&& bsdfB = bsdfs.find(bsdfBName);

    CADISE_ASSERT_NE(bsdfB, bsdfs.end());

    const real ratio = data->findReal("ratio", 0.7_r);

    return std::make_shared<MixedBsdf>(bsdfA->second,
                                       bsdfB->second,
                                       ratio);
}

static std::shared_ptr<Bsdf> createConductorMicrofacet(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const std::string_view microfacetType = data->findString("microfacet-type", "ggx");
    const std::string_view fresnelType    = data->findString("fresnel-type", "schlick");
    const std::shared_ptr<Texture<real>> roughness
        = data->getRealTexture("roughness", realTextures);

    std::shared_ptr<Microfacet> microfacet = nullptr;
    if (microfacetType == "beckmann") {
        microfacet = std::make_shared<IsotropicBeckmann>();
    }
    else if (microfacetType == "ggx") {
        microfacet = std::make_shared<IsotropicTrowbridgeReitz>();
    }
    else if (microfacetType == "blinn-phong") {
        microfacet = std::make_shared<IsotropicBlinnPhong>();
    }
    else {
        microfacet = std::make_shared<IsotropicTrowbridgeReitz>();
    }

    std::shared_ptr<ConductorFresnel> fresnel = nullptr;
    if (fresnelType == "schlick") {
        const Vector3R f0 = data->findVector3r("f0");
        fresnel = std::make_shared<SchlickConductorFresnel>(Spectrum(f0));
    }
    else {
        const Vector3R f0 = data->findVector3r("f0");
        fresnel = std::make_shared<SchlickConductorFresnel>(Spectrum(f0));
    }

    return std::make_shared<ConductorMicrofacet>(microfacet, fresnel, roughness);
}

std::shared_ptr<Bsdf> makeBsdf(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures,
    const StringKeyMap<Bsdf>& bsdfs) {

    CADISE_ASSERT(data);

    std::shared_ptr<Bsdf> bsdf = nullptr;

    const std::string_view type = data->findString("type");
    if (type == "matte-lambertian") {
        bsdf = createLambertianDiffuse(data, realTextures, spectrumTextures);
    }
    else if (type == "mirror") {
        bsdf = createSpecularReflection(data, realTextures, spectrumTextures);
    }
    else if (type == "transmission") {
        bsdf = createSpecularTransmission(data, realTextures, spectrumTextures);
    }
    else if (type == "glass") {
        bsdf = createSpecularDielectric(data, realTextures, spectrumTextures);
    }
    else if (type == "phong") {
        bsdf = createPhong(data, realTextures, spectrumTextures);
    }
    else if (type == "plastic") {
        bsdf = createPlastic(data, realTextures, spectrumTextures);
    }
    else if (type == "mixed") {
        bsdf = createMixed(data, realTextures, spectrumTextures, bsdfs);
    }
    else if (type == "conductor-microfacet") {
        bsdf = createConductorMicrofacet(data, realTextures, spectrumTextures);
    }
    else if (type == "absorb") {
        bsdf = std::make_shared<AbsorberBsdf>();
    }
    else {
        // unsupported bsdf type
        std::cout << "Unsupported bsdf type: " << type << std::endl;
    }

    return bsdf;
}

} // namespace instantiator

} // namespace cadise