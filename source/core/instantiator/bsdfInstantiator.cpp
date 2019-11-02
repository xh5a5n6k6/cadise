#include "core/instantiator/instantiator.h"

// bsdf type
#include "core/bsdf/category/blinnPhong.h"
#include "core/bsdf/category/lambertianDiffuse.h"
#include "core/bsdf/category/mixedBsdf.h"
#include "core/bsdf/category/perfectDielectric.h"
#include "core/bsdf/category/specularReflection.h"
#include "core/bsdf/category/specularTransmission.h"

#include "core/texture/texture.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Bsdf> createLambertianDiffuse(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo = data->getSpectrumTexture("albedo", spectrumTextures);

    return std::make_shared<LambertianDiffuse>(albedo);
}

static std::shared_ptr<Bsdf> createSpecularReflection(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo = data->getSpectrumTexture("albedo", spectrumTextures);

    return std::make_shared<SpecularReflection>(albedo);
}

static std::shared_ptr<Bsdf> createSpecularTransmission(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo   = data->getSpectrumTexture("albedo", spectrumTextures);
    const real                               iorOuter = data->findReal("ior-outer", 1.0_r);
    const real                               iorInner = data->findReal("ior-inner", 1.5_r);

    return std::make_shared<SpecularTransmission>(albedo, iorOuter, iorInner);
}

static std::shared_ptr<Bsdf> createPerfectDielectric(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> albedo   = data->getSpectrumTexture("albedo", spectrumTextures);
    const real                               iorOuter = data->findReal("ior-outer", 1.0_r);
    const real                               iorInner = data->findReal("ior-inner", 1.5_r);

    return std::make_shared<PerfectDielectric>(albedo, iorOuter, iorInner);
}

static std::shared_ptr<Bsdf> createBlinnPhong(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const real exponent = data->findReal("exponent", 32.0_r);

    return std::make_shared<BlinnPhong>(exponent);
}

static std::shared_ptr<Bsdf> createPlastic(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const std::shared_ptr<Texture<Spectrum>> diffuseAlbedo
        = data->getSpectrumTexture("diffuse-albedo", spectrumTextures);
    const real specularExponent = data->findReal("specular-exponent", 32.0_r);
    const real diffuseRatio     = data->findReal("diffuse-ratio", 0.7_r);

    return std::make_shared<MixedBsdf>(std::make_shared<LambertianDiffuse>(diffuseAlbedo),
                                       std::make_shared<BlinnPhong>(specularExponent),
                                       diffuseRatio);
}

std::shared_ptr<Bsdf> makeBsdf(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

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
        bsdf = createPerfectDielectric(data, realTextures, spectrumTextures);
    }
    else if (type == "blinnPhong") {
        bsdf = createBlinnPhong(data, realTextures, spectrumTextures);
    }
    else if (type == "plastic") {
        bsdf = createPlastic(data, realTextures, spectrumTextures);
    }
    else {
        // don't support bsdf type
    }

    return bsdf;
}

} // namespace instantiator

} // namespace cadise