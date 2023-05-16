#pragma once

#include "Math/Type/ImageType.h"

// forward declaration
namespace cadise
{
    class Path;
}

namespace cadise
{

class PictureLoader
{
public:
    static HDRImage loadRgbImage(const Path& path);
    static HDRAlphaImage loadRgbaImage(const Path& path);

private:
    static void _loadLdrImage(const Path& path, LDRImage* const out_ldrImage);
    static void _loadHdrImage(const Path& path, HDRImage* const out_hdrImage);

    static void _loadLdrAlphaImage(
        const Path&          path,
        LDRAlphaImage* const out_ldrAlphaImage);

    static void _loadHdrAlphaImage(
        const Path&          path,
        HDRAlphaImage* const out_hdrAlphaImage);
};

} // namespace cadise