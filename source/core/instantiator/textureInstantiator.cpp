#include "core/instantiator/instantiator.h"

#include "core/imaging/image.h"

// texture type
#include "core/texture/category/constantTexture.h"
#include "core/texture/category/checkerboardTexture.h"
#include "core/texture/category/realImageTexture.h"
#include "core/texture/category/rgbaImageTexture.h"
#include "core/texture/category/rgbImageTexture.h"
#include "core/texture/textureSamplingMode.h"

#include "file-io/path.h"
#include "file-io/pictureLoader.h"
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
    const Vector3R value = data->findVector3r("value");

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

static std::shared_ptr<Texture<Spectrum>> createSpectrumImage(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {

    const std::string_view filename = data->findString("filename");
    const std::string_view samplingMode = data->findString("sampling-mode");

    const HdrImage hdrImage = PictureLoader::loadRgbImage(Path(filename));

    TextureSamplingMode mode;
    if (!samplingMode.compare("nearest")) {
        mode = TextureSamplingMode::NEAREST;
    }
    else {
        mode = TextureSamplingMode::NEAREST;
    }

    return std::make_shared<RgbImageTexture>(hdrImage, mode);
}

//static std::shared_ptr<Texture<Spectrum>> createSpectrumAlphaImage(
//    const std::shared_ptr<SdData>& data,
//    const std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>& realTextures,
//    const std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>>& spectrumTextures) {
//
//    const std::string_view filename = data->findString("filename");
//
//    const HdrAlphaImage hdrAlphaImage = PictureLoader::loadRgbaImage(Path(filename));
//
//    return std::make_shared<RgbaImageTexture>(hdrAlphaImage);
//}

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
    else if (!type.compare("image")) {
        // TODO: add image texture
        //realTexture = createRealImage(data, realTextures, spectrumTextures);
    }
    else if (!type.compare("alpha-image")) {
        // TODO: add alpha image texture
        //realTexture = createRealAlphaImage(data, realTextures, spectrumTextures);
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
    else if (!type.compare("image")) {
        spectrumTexture = createSpectrumImage(data, realTextures, spectrumTextures);
    }
    else if (!type.compare("alpha-image")) {
        // TODO: add alpha image texture
        //spectrumTexture = createSpectrumAlphaImage(data, realTextures, spectrumTextures);
    }
    else {
        // don't support texture type
    }

    return spectrumTexture;
}

} // namespace instantiator

} // namespace cadise