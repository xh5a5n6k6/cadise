#pragma once

#include "Math/Type/ImageType.h"

namespace cadise
{

class ImageUtility
{
public:
    static void ldrToHdr(
        const LDRImage& ldrImage,
        HDRImage* const out_hdrImage);

    static void hdrToLdr(
        const HDRImage& hdrImage,
        LDRImage* const out_ldrImage);

    static void ldrAlphaToHdrAlpha(
        const LDRAlphaImage& ldrAlphaImage,
        HDRAlphaImage* const out_hdrAlphaImage);
};

} // namespace cadise