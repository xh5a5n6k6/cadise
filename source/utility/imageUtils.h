#pragma once

#include "math/type/imageType.h"

namespace cadise {

namespace utility::image {

void ldrToHdr(const LdrImage& ldrImage, HdrImage* const out_hdrImage);

void hdrToLdr(const HdrImage& hdrImage, LdrImage* const out_ldrImage);

void ldrAlphaToHdrAlpha(const LdrAlphaImage& ldrAlphaImage, HdrAlphaImage* const out_hdrAlphaImage);

} // namespace utility::image

} // namespace cadise