#pragma once

#include "math/type/imageType.h"

namespace cadise { class Path; }

namespace cadise 
{

class PictureSaver 
{
public:
    static void save(const Path& path, const HdrImage& hdrImage);

private:
    static void saveLdrImage(const Path& path, const LdrImage& ldrImage);
    static void saveHdrImage(const Path& path, const HdrImage& hdrImage);

    static bool savePNG(const Path& path, const LdrImage& ldrImage);
    static bool saveJPG(const Path& path, const LdrImage& ldrImage);
    static bool savePPM(const Path& path, const LdrImage& ldrImage);
};

} // namespace cadise