#include "core/instantiator/instantiator.h"

// texture type
#include "core/texture/category/tConstantTexture.h"
#include "core/texture/category/tCheckerboardTexture.h"
#include "core/texture/category/realImageTexture.h"
#include "core/texture/category/rgbaImageTexture.h"
#include "core/texture/category/rgbImageTexture.h"

#include "core/imaging/tImage.h"
#include "core/texture/eTextureSampleMode.h"
#include "core/texture/eTextureWrapMode.h"
#include "file-io/path.h"
#include "file-io/pictureLoader.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<TTexture<real>> createRealConstant(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures) {

    const real value = data->findReal("value");

    return std::make_shared<TConstantTexture<real>>(value);
}

static std::shared_ptr<TTexture<real>> createRealCheckerboard(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures) {

    const real oddNumber   = data->findReal("odd-number");
    const real evenNumber  = data->findReal("even-number");
    const auto oddTexture  = data->getRealTexture("odd-texture", realTextures);
    const auto evenTexture = data->getRealTexture("even-texture", realTextures);

    return std::make_shared<TCheckerboardTexture<real>>(
        oddNumber, evenNumber, oddTexture, evenTexture);
}

static std::shared_ptr<TTexture<Spectrum>> createSpectrumConstant(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures) {

    // it now only support rgb spectrum
    const auto value = data->findVector3r("value");

    return std::make_shared<TConstantTexture<Spectrum>>(Spectrum(value));
}

static std::shared_ptr<TTexture<Spectrum>> createSpectrumCheckerboard(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures) {

    const real oddNumber   = data->findReal("odd-number");
    const real evenNumber  = data->findReal("even-number");
    const auto oddTexture  = data->getSpectrumTexture("odd-texture", spectrumTextures);
    const auto evenTexture = data->getSpectrumTexture("even-texture", spectrumTextures);

    return std::make_shared<TCheckerboardTexture<Spectrum>>(
        oddNumber, evenNumber, oddTexture, evenTexture);
}

static std::shared_ptr<TTexture<Spectrum>> createSpectrumImage(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures) {

    const auto filename = data->findString("filename");
    const auto sMode    = data->findString("sample-mode");
    const auto wMode    = data->findString("wrap-mode");

    HdrImage hdrImage = PictureLoader::loadRgbImage(Path(filename));

    ETextureSampleMode sampleMode;
    if (sMode == "nearest") {
        sampleMode = ETextureSampleMode::NEAREST;
    }
    // TODO: add bilinear sample mode
    else {
        sampleMode = ETextureSampleMode::NEAREST;
    }

    ETextureWrapMode wrapMode;
    if (wMode == "clamp") {
        wrapMode = ETextureWrapMode::CLAMP;
    }
    else if (wMode == "repeat") {
        wrapMode = ETextureWrapMode::REPEAT;
    }
    else {
        wrapMode = ETextureWrapMode::REPEAT;
    }

    return std::make_shared<RgbImageTexture>(hdrImage, sampleMode, wrapMode);
}

//static std::shared_ptr<Texture<Spectrum>> createSpectrumAlphaImage(
//    const std::shared_ptr<SdData>& data,
//    const StringKeyMap<Texture<real>>& realTextures,
//    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {
//
//    const std::string_view filename = data->findString("filename");
//
//    const HdrAlphaImage hdrAlphaImage = PictureLoader::loadRgbaImage(Path(filename));
//
//    return std::make_shared<RgbaImageTexture>(hdrAlphaImage);
//}

std::shared_ptr<TTexture<real>> makeRealTexture(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures) {

    CADISE_ASSERT(data);

    std::shared_ptr<TTexture<real>> realTexture = nullptr;

    const auto type = data->findString("type");
    if (type == "constant") {
        realTexture = createRealConstant(data, realTextures, spectrumTextures);
    }
    else if (type == "checkerboard") {
        realTexture = createRealCheckerboard(data, realTextures, spectrumTextures);
    }
    else if (type == "image") {
        // TODO: add image texture
        //realTexture = createRealImage(data, realTextures, spectrumTextures);
    }
    else if (type == "alpha-image") {
        // TODO: add alpha image texture
        //realTexture = createRealAlphaImage(data, realTextures, spectrumTextures);
    }
    else {
        // don't support texture type
        std::cout << "Unsupported texture type: <" << type << ">" << std::endl;
    }

    return realTexture;
}

std::shared_ptr<TTexture<Spectrum>> makeSpectrumTexture(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures) {

    CADISE_ASSERT(data);

    std::shared_ptr<TTexture<Spectrum>> spectrumTexture = nullptr;

    const auto type = data->findString("type");
    if (type == "constant") {
        spectrumTexture = createSpectrumConstant(data, realTextures, spectrumTextures);
    }
    else if (type == "checkerboard") {
        spectrumTexture = createSpectrumCheckerboard(data, realTextures, spectrumTextures);
    }
    else if (type == "image") {
        spectrumTexture = createSpectrumImage(data, realTextures, spectrumTextures);
    }
    else if (type == "alpha-image") {
        // TODO: add alpha image texture
        //spectrumTexture = createSpectrumAlphaImage(data, realTextures, spectrumTextures);
    }
    else {
        // unsupported texture type
        std::cout << "Unsupported texture type: <" << type << ">" << std::endl;
    }

    return spectrumTexture;
}

} // namespace instantiator

} // namespace cadise