#include "FileIO/PictureLoader.h"

#include "Core/Image/TImage.h"
#include "FileIO/Path.h"
#include "Foundation/Assertion.h"
#include "Math/MathUtility.h"
#include "Math/TVector3.h"
#include "ThirdParty/TP_stb-load.h"
#include "FileIO/ImageUtility.h"

#include <iostream>

namespace cadise
{

HDRImage PictureLoader::loadRgbImage(const Path& path)
{
    HDRImage hdrImage;
    if (path.isExtendedWith(".jpg") || path.isExtendedWith(".JPG") ||
        path.isExtendedWith(".png") || path.isExtendedWith(".PNG") ||
        path.isExtendedWith(".tga") || path.isExtendedWith(".TGA"))
    {
        LDRImage ldrImage;
        _loadLdrImage(path, &ldrImage);
        ImageUtility::ldrToHdr(ldrImage, &hdrImage);
    }
    else if (path.isExtendedWith(".hdr") || path.isExtendedWith(".HDR"))
    {
        _loadHdrImage(path, &hdrImage);
    }

    return hdrImage;
}

HDRAlphaImage PictureLoader::loadRgbaImage(const Path& path)
{
    HDRAlphaImage hdrAlphaImage;

    // TODO: implement here

    return hdrAlphaImage;
}

void PictureLoader::_loadLdrImage(const Path& path, LDRImage* const out_ldrImage)
{
    CS_ASSERT(out_ldrImage);

    int32 width;
    int32 height;
    int32 componentNumber;

    // stbi's origin is at left-up corner, but image's uv is
    // starting from left-down corner, so we need to do
    // this operation to fit the situation.
    stbi_set_flip_vertically_on_load(true);

    stbi_uc* imageData = stbi_load(
        path.path().c_str(),
        &width,
        &height,
        &componentNumber,
        0);

    if (imageData == NULL)
    {
        return;
    }

    out_ldrImage->setImageSize(width, height);

    // gray image, we still store 3 components each pixel with same value
    if (componentNumber == 1)
    {
        for (int32 iy = 0; iy < height; ++iy)
        {
            for (int32 ix = 0; ix < width; ++ix)
            {
                const std::size_t indexOffset     = static_cast<std::size_t>(ix + iy * width);
                const std::size_t dataIndexOffset = indexOffset * 3;

                // TODO: transform input sRGB to linear-sRGB
                //       ldr -> hdr -> inverseGamma -> ldr
                real value       = static_cast<real>(imageData[indexOffset]) / 255.0_r;
                real linearValue = MathUtility::inverseGammaCorrection(value);
                linearValue = linearValue * 255.0_r + 0.5_r;

                const uint8 ldrLinearValue = static_cast<uint8>(MathUtility::clamp(linearValue, 0.0_r, 255.0_r));

                out_ldrImage->setDataValue(dataIndexOffset + 0, ldrLinearValue);
                out_ldrImage->setDataValue(dataIndexOffset + 1, ldrLinearValue);
                out_ldrImage->setDataValue(dataIndexOffset + 2, ldrLinearValue);
            }
        }
    }
    // rgb image
    else if (componentNumber == 3)
    {
        for (int32 iy = 0; iy < height; ++iy)
        {
            for (int32 ix = 0; ix < width; ++ix)
            {
                const std::size_t indexOffset = static_cast<std::size_t>((ix + iy * width) * 3);

                // TODO: transform input sRGB to linear-sRGB as a function
                //       ldr -> hdr -> inverseGamma -> ldr
                real r = static_cast<real>(imageData[indexOffset + 0]) / 255.0_r;
                real g = static_cast<real>(imageData[indexOffset + 1]) / 255.0_r;
                real b = static_cast<real>(imageData[indexOffset + 2]) / 255.0_r;

                Vector3R linearRgb(
                    MathUtility::inverseGammaCorrection(r),
                    MathUtility::inverseGammaCorrection(g),
                    MathUtility::inverseGammaCorrection(b));

                linearRgb.mulLocal(255.0_r).addLocal(0.5_r);

                const uint8 linearR = static_cast<uint8>(MathUtility::clamp(linearRgb.x(), 0.0_r, 255.0_r));
                const uint8 linearG = static_cast<uint8>(MathUtility::clamp(linearRgb.y(), 0.0_r, 255.0_r));
                const uint8 linearB = static_cast<uint8>(MathUtility::clamp(linearRgb.z(), 0.0_r, 255.0_r));

                out_ldrImage->setDataValue(indexOffset + 0, linearR);
                out_ldrImage->setDataValue(indexOffset + 1, linearG);
                out_ldrImage->setDataValue(indexOffset + 2, linearB);
            }
        }
    }
    // image with alpha channel
    else if (componentNumber == 2 || componentNumber == 4)
    {
        std::cerr << "[" << path.path() << "]"
            << "has alpha channel, please modify CRSD file instead."
            << std::endl;
    }
    else
    {
        // Something goes wrong
    }

    stbi_image_free(imageData);
}

void PictureLoader::_loadHdrImage(const Path& path, HDRImage* const out_hdrImage)
{
    CS_ASSERT(out_hdrImage);

    int32 width;
    int32 height;
    int32 componentNumber;

    // stbi's origin is at left-up corner, but image's uv is
    // starting from left-down corner, so we need to do
    // this operation to fit the situation.
    stbi_set_flip_vertically_on_load(true);

    float* imageData = stbi_loadf(
        path.path().c_str(),
        &width,
        &height,
        &componentNumber,
        0);

    if (imageData == NULL)
    {
        return;
    }

    out_hdrImage->setImageSize(width, height);

    // gray image, we still store 3 components each pixel with same value
    if (componentNumber == 1)
    {
        for (int32 iy = 0; iy < height; ++iy)
        {
            for (int32 ix = 0; ix < width; ++ix)
            {
                const std::size_t indexOffset = static_cast<std::size_t>(ix + iy * width);
                const real        pixelValue  = imageData[indexOffset];

                out_hdrImage->setPixelValue(ix, iy, { pixelValue, pixelValue, pixelValue });
            }
        }
    }
    // rgb image
    else if (componentNumber == 3)
    {
        for (int32 iy = 0; iy < height; ++iy)
        {
            for (int32 ix = 0; ix < width; ++ix)
            {
                const std::size_t indexOffset = static_cast<std::size_t>((ix + iy * width) * 3);

                out_hdrImage->setPixelValue(
                    ix,
                    iy,
                    {
                        static_cast<real>(imageData[indexOffset + 0]),
                        static_cast<real>(imageData[indexOffset + 1]),
                        static_cast<real>(imageData[indexOffset + 2])
                    });
            }
        }
    }
    else
    {

    }

    stbi_image_free(imageData);
}

void PictureLoader::_loadLdrAlphaImage(const Path& path, LDRAlphaImage* const out_ldrAlphaImage)
{
    CS_ASSERT(out_ldrAlphaImage);
    // TODO: implement here
}

void PictureLoader::_loadHdrAlphaImage(const Path& path, HDRAlphaImage* const out_hdrAlphaImage)
{
    CS_ASSERT(out_hdrAlphaImage);
    // TODO: implement here
}

} // namespace cadise