#pragma once

#include "math/type/imageType.h"

namespace cadise { class Path; }

namespace cadise 
{

class PictureLoader 
{
public:
    static HdrImage      loadRgbImage(const Path& path);
    static HdrAlphaImage loadRgbaImage(const Path& path);

private:
    static void loadLdrImage(const Path& path, LdrImage* const out_ldrImage);
    static void loadHdrImage(const Path& path, HdrImage* const out_hdrImage);

    static void loadLdrAlphaImage(
        const Path&          path, 
        LdrAlphaImage* const out_ldrAlphaImage);

    static void loadHdrAlphaImage(
        const Path&          path, 
        HdrAlphaImage* const out_hdrAlphaImage);
};

} // namespace cadise