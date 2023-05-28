#include "FileIO/ImageUtility.h"

#include "Core/Image/TImage.h"
#include "Foundation/Assertion.h"
#include "Math/MathUtility.h"

namespace cadise
{

void ImageUtility::ldrToHdr(
    const LDRImage& ldrImage,
    HDRImage* const out_hdrImage)
{
    CS_ASSERT(out_hdrImage);

    out_hdrImage->setImageSize(ldrImage.width(), ldrImage.height());

    const uint8* data = ldrImage.rawData();
    for (std::size_t i = 0; i < ldrImage.dataSize(); ++i)
    {
        const real value = static_cast<real>(data[i]) / 255.0_r;
        out_hdrImage->setDataValue(i, value);
    }
}

void ImageUtility::hdrToLdr(
    const HDRImage& hdrImage,
    LDRImage* const out_ldrImage)
{
    CS_ASSERT(out_ldrImage);

    out_ldrImage->setImageSize(hdrImage.width(), hdrImage.height());

    const real* data = hdrImage.rawData();
    for (std::size_t i = 0; i < hdrImage.dataSize(); ++i)
    {
        const uint8 value = static_cast<uint8>(
            MathUtility::clamp(data[i] * 255.0_r + 0.5_r, 0.0_r, 255.0_r));
        out_ldrImage->setDataValue(i, value);
    }
}

void ImageUtility::ldrAlphaToHdrAlpha(
    const LDRAlphaImage& ldrAlphaImage,
    HDRAlphaImage* const out_hdrAlphaImage)
{
    CS_ASSERT(out_hdrAlphaImage);

    out_hdrAlphaImage->setImageSize(ldrAlphaImage.width(), ldrAlphaImage.height());

    // TODO: implement here
}

} // namespace cadise