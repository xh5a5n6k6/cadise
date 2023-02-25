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

namespace cadise::instantiator 
{

static std::shared_ptr<TTexture<real>> createRealConstant(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const real value = data->findFloat<real>("value");

    return std::make_shared<TConstantTexture<real>>(value);
}

static std::shared_ptr<TTexture<real>> createRealCheckerboard(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const real oddNumber   = data->findFloat<real>("odd-number");
    const real evenNumber  = data->findFloat<real>("even-number");
    const auto oddTexture  = data->getRealTexture("odd-texture", realTextures);
    const auto evenTexture = data->getRealTexture("even-texture", realTextures);

    return std::make_shared<TCheckerboardTexture<real>>(
        oddNumber, evenNumber, oddTexture, evenTexture);
}

static std::shared_ptr<TTexture<Spectrum>> createSpectrumConstant(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    // it now only support rgb spectrum
    const auto value = data->findVector3<real>("value");

    return std::make_shared<TConstantTexture<Spectrum>>(Spectrum(value));
}

static std::shared_ptr<TTexture<Spectrum>> createSpectrumCheckerboard(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const real oddNumber   = data->findFloat<real>("odd-number");
    const real evenNumber  = data->findFloat<real>("even-number");
    const auto oddTexture  = data->getSpectrumTexture("odd-texture", spectrumTextures);
    const auto evenTexture = data->getSpectrumTexture("even-texture", spectrumTextures);

    return std::make_shared<TCheckerboardTexture<Spectrum>>(
        oddNumber, evenNumber, oddTexture, evenTexture);
}

static std::shared_ptr<TTexture<Spectrum>> createSpectrumImage(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    const auto filename = data->findString("filename");
    const auto sMode    = data->findString("sample-mode");
    const auto wMode    = data->findString("wrap-mode");

    HdrImage hdrImage = PictureLoader::loadRgbImage(Path(filename));

    ETextureSampleMode sampleMode;
    if (sMode == "nearest")
    {
        sampleMode = ETextureSampleMode::Nearest;
    }
    // TODO: add bilinear sample mode
    else
    {
        sampleMode = ETextureSampleMode::Nearest;
    }

    ETextureWrapMode wrapMode;
    if (wMode == "clamp") 
    {
        wrapMode = ETextureWrapMode::Clamp;
    }
    else if (wMode == "repeat")
    {
        wrapMode = ETextureWrapMode::Repeat;
    }
    else 
    {
        wrapMode = ETextureWrapMode::Repeat;
    }

    return std::make_shared<RgbImageTexture>(hdrImage, sampleMode, wrapMode);
}

//static std::shared_ptr<Texture<Spectrum>> createSpectrumAlphaImage(
//    const std::shared_ptr<SdData>& data,
//    const TStringKeyMap<Texture<real>>& realTextures,
//    const TStringKeyMap<Texture<Spectrum>>& spectrumTextures) {
//
//    const std::string_view filename = data->findString("filename");
//
//    const HdrAlphaImage hdrAlphaImage = PictureLoader::loadRgbaImage(Path(filename));
//
//    return std::make_shared<RgbaImageTexture>(hdrAlphaImage);
//}

std::shared_ptr<TTexture<real>> makeRealTexture(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    CS_ASSERT(data);

    std::shared_ptr<TTexture<real>> realTexture = nullptr;

    const auto type = data->findString("type");
    if (type == "constant")
    {
        realTexture = createRealConstant(data, realTextures, spectrumTextures);
    }
    else if (type == "checkerboard")
    {
        realTexture = createRealCheckerboard(data, realTextures, spectrumTextures);
    }
    else if (type == "image") 
    {
        // TODO: add image texture
        //realTexture = createRealImage(data, realTextures, spectrumTextures);
    }
    else if (type == "alpha-image") 
    {
        // TODO: add alpha image texture
        //realTexture = createRealAlphaImage(data, realTextures, spectrumTextures);
    }
    else 
    {
        // don't support texture type
        std::cout << "Unsupported texture type: <" << type << ">" << std::endl;
    }

    return realTexture;
}

std::shared_ptr<TTexture<Spectrum>> makeSpectrumTexture(
    const std::shared_ptr<SdData>&           data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures)
{
    CS_ASSERT(data);

    std::shared_ptr<TTexture<Spectrum>> spectrumTexture = nullptr;

    const auto type = data->findString("type");
    if (type == "constant")
    {
        spectrumTexture = createSpectrumConstant(data, realTextures, spectrumTextures);
    }
    else if (type == "checkerboard")
    {
        spectrumTexture = createSpectrumCheckerboard(data, realTextures, spectrumTextures);
    }
    else if (type == "image") 
    {
        spectrumTexture = createSpectrumImage(data, realTextures, spectrumTextures);
    }
    else if (type == "alpha-image") 
    {
        // TODO: add alpha image texture
        //spectrumTexture = createSpectrumAlphaImage(data, realTextures, spectrumTextures);
    }
    else 
    {
        // unsupported texture type
        std::cout << "Unsupported texture type: <" << type << ">" << std::endl;
    }

    return spectrumTexture;
}

} // namespace cadise::instantiator