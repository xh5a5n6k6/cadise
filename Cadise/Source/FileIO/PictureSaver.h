#pragma once

#include "Math/Type/ImageType.h"

// forward declaration
namespace cadise
{
    class Path;
}

namespace cadise 
{

class PictureSaver 
{
public:
    static void save(const Path& path, const HDRImage& hdrImage);

private:
    static void _saveLdrImage(const Path& path, const LDRImage& ldrImage);
    static void _saveHdrImage(const Path& path, const HDRImage& hdrImage);

    static bool _savePNG(const Path& path, const LDRImage& ldrImage);
    static bool _saveJPG(const Path& path, const LDRImage& ldrImage);
    static bool _savePPM(const Path& path, const LDRImage& ldrImage);
};

} // namespace cadise