#include "core/instantiator/instantiator.h"

// texture type
#include "core/texture/category/constantTexture.h"
#include "core/texture/category/checkerboardTexture.h"
#include "core/texture/category/realImageTexture.h"
#include "core/texture/category/rgbaImageTexture.h"
#include "core/texture/category/rgbImageTexture.h"

#include "core/imaging/image.h"
#include "core/texture/textureSampleMode.h"
#include "core/texture/textureWrapMode.h"
#include "file-io/path.h"
#include "file-io/pictureLoader.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Texture<real>> createRealConstant(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const real value = data->findReal("value");

    return std::make_shared<ConstantTexture<real>>(value);
}

static std::shared_ptr<Texture<real>> createRealCheckerboard(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

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
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    // it now only support rgb spectrum
    const Vector3R value = data->findVector3r("value");

    return std::make_shared<ConstantTexture<Spectrum>>(value);
}

static std::shared_ptr<Texture<Spectrum>> createSpectrumCheckerboard(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

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
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    const std::string_view filename = data->findString("filename");
    const std::string_view sMode    = data->findString("sample-mode");
    const std::string_view wMode    = data->findString("wrap-mode");

    HdrImage hdrImage = PictureLoader::loadRgbImage(Path(filename));

    TextureSampleMode sampleMode;
    if (sMode == "nearest") {
        sampleMode = TextureSampleMode::NEAREST;
    }
    // TODO: add bilinear sample mode
    else {
        sampleMode = TextureSampleMode::NEAREST;
    }

    TextureWrapMode wrapMode;
    if (wMode == "clamp") {
        wrapMode = TextureWrapMode::CLAMP;
    }
    else if (wMode == "repeat") {
        wrapMode = TextureWrapMode::REPEAT;
    }
    else {
        wrapMode = TextureWrapMode::REPEAT;
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

std::shared_ptr<Texture<real>> makeRealTexture(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    CADISE_ASSERT(data);

    std::shared_ptr<Texture<real>> realTexture = nullptr;

    const std::string_view type = data->findString("type");
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
    }

    return realTexture;
}

std::shared_ptr<Texture<Spectrum>> makeSpectrumTexture(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures) {

    CADISE_ASSERT(data);

    std::shared_ptr<Texture<Spectrum>> spectrumTexture = nullptr;

    const std::string_view type = data->findString("type");
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
    }

    return spectrumTexture;
}

} // namespace instantiator

} // namespace cadise