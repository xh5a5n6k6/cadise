#pragma once

#include "math/type/imageType.h"

namespace cadise {

class Path;

class PictureLoader {
public:
    static HdrImage      loadRgbImage(const Path& path);
    static HdrAlphaImage loadRgbaImage(const Path& path);

    static void loadLdrImage(const Path& path, LdrImage* const out_ldrImage);
    static void loadLdrAlphaImage(const Path& path, LdrAlphaImage* const out_ldrAlphaImage);
};

} // namespace cadise