#include "utility/imageUtils.h"

#include "core/imaging/image.h"
#include "fundamental/assertion.h"
#include "math/math.h"

namespace cadise {

void ImageUtils::ldrToHdr(const LdrImage& ldrImage, 
                          HdrImage* const out_hdrImage) {

    CADISE_ASSERT(out_hdrImage);

    out_hdrImage->setImageSize(ldrImage.width(), ldrImage.height());

    const uint8* data = ldrImage.rawData();
    for (std::size_t i = 0; i < ldrImage.dataSize(); ++i) {
        const real value = static_cast<real>(data[i]) / 255.0_r;
        out_hdrImage->setDataValue(i, value);
    }
}

void ImageUtils::hdrToLdr(const HdrImage& hdrImage, 
                          LdrImage* const out_ldrImage) {

    CADISE_ASSERT(out_ldrImage);

    out_ldrImage->setImageSize(hdrImage.width(), hdrImage.height());

    const real* data = hdrImage.rawData();
    for (std::size_t i = 0; i < hdrImage.dataSize(); ++i) {
        const uint8 value = static_cast<uint8>(
            math::clamp(data[i] * 255.0_r + 0.5_r, 0.0_r, 255.0_r));
        out_ldrImage->setDataValue(i, value);
    }
}

void ImageUtils::ldrAlphaToHdrAlpha(const LdrAlphaImage& ldrAlphaImage, 
                                    HdrAlphaImage* const out_hdrAlphaImage) {

    CADISE_ASSERT(out_hdrAlphaImage);

    out_hdrAlphaImage->setImageSize(ldrAlphaImage.width(), ldrAlphaImage.height());

    // TODO: implement here
}

} // namespace cadise