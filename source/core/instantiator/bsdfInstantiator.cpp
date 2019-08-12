#include "core/instantiator/instantiator.h"

// bsdf type
#include "core/bsdf/category/blinnPhong.h"
#include "core/bsdf/category/lambertianDiffuse.h"
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

    const std::shared_ptr<Texture<Spectrum>> albedo = data->getSpectrumTexture("albedo", spectrumTextures);
    const real                               iorOuter = data->findReal("ior-outer", 1.0_r);
    const real                               iorInner = data->findReal("ior-inner", 1.5_r);

    return std::make_shared<PerfectDielectric>(albedo, iorOuter, iorInner);
}

std::shared_ptr<Bsdf> makeBsdf(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    std::shared_ptr<Bsdf> bsdf = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("matte-lambertian")) {
        bsdf = createLambertianDiffuse(data, realTextures, spectrumTextures);
    }
    else if (!type.compare("mirror")) {
        bsdf = createSpecularReflection(data, realTextures, spectrumTextures);
    }
    else if (!type.compare("transmission")) {
        bsdf = createSpecularTransmission(data, realTextures, spectrumTextures);
    }
    else if (!type.compare("glass")) {
        bsdf = createPerfectDielectric(data, realTextures, spectrumTextures);
    }
    else {
        // don't support primitive type
    }

    return bsdf;
}

} // namespace instantiator

} // namespace cadise