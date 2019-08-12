#include "core/instantiator/instantiator.h"

// texture type
#include "core/texture/category/constantTexture.h"
#include "core/texture/category/checkerboardTexture.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Texture<real>> createRealConstant(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const real value = data->findReal("value");

    return std::make_shared<ConstantTexture<real>>(value);
}

static std::shared_ptr<Texture<real>> createRealCheckerboard(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const real oddNumber  = data->findReal("odd-number");
    const real evenNumber = data->findReal("even-number");
    const std::shared_ptr<Texture<real>> oddTexture  
        = data->getRealTexture("odd-texture", realTextures);
    const std::shared_ptr<Texture<real>> evenTexture 
        = data->getRealTexture("even-texture", realTextures);

    return std::make_shared<CheckerboardTexture<real>>(oddNumber, evenNumber,
                                                       oddTexture, evenTexture);
}

static std::shared_ptr<Texture<Spectrum>> createSpectrumConstant(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    // it now only support rgb spectrum
    const Vector3R value = data->findVector3R("value");

    return std::make_shared<ConstantTexture<Spectrum>>(value);
}

static std::shared_ptr<Texture<Spectrum>> createSpectrumCheckerboard(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const real oddNumber  = data->findReal("odd-number");
    const real evenNumber = data->findReal("even-number");
    const std::shared_ptr<Texture<Spectrum>> oddTexture  
        = data->getSpectrumTexture("odd-texture", spectrumTextures);
    const std::shared_ptr<Texture<Spectrum>> evenTexture 
        = data->getSpectrumTexture("even-texture", spectrumTextures);

    return std::make_shared<CheckerboardTexture<Spectrum>>(oddNumber, evenNumber,
                                                           oddTexture, evenTexture);
}

std::shared_ptr<Texture<real>> makeRealTexture(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    std::shared_ptr<Texture<real>> realTexture = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("constant")) {
        realTexture = createRealConstant(data, realTextures, spectrumTextures);
    }
    else if (!type.compare("checkerboard")) {
        realTexture = createRealCheckerboard(data, realTextures, spectrumTextures);
    }
    else {
        // don't support texture type
    }

    return realTexture;
}

std::shared_ptr<Texture<Spectrum>> makeSpectrumTexture(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    std::shared_ptr<Texture<Spectrum>> spectrumTexture = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("constant")) {
        spectrumTexture = createSpectrumConstant(data, realTextures, spectrumTextures);
    }
    else if (!type.compare("checkerboard")) {
        spectrumTexture = createSpectrumCheckerboard(data, realTextures, spectrumTextures);
    }
    else {
        // don't support texture type
    }

    return spectrumTexture;
}

} // namespace instantiator

} // namespace cadise