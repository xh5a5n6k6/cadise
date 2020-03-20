#pragma once

#include "math/type/imageType.h"

namespace cadise {

class ImageUtils {
public:
    static void ldrToHdr(const LdrImage& ldrImage, 
                         HdrImage* const out_hdrImage);

    static void hdrToLdr(const HdrImage& hdrImage, 
                         LdrImage* const out_ldrImage);

    static void ldrAlphaToHdrAlpha(const LdrAlphaImage& ldrAlphaImage, 
                                   HdrAlphaImage* const out_hdrAlphaImage);
};

} // namespace cadise