#include "FileIO/PictureSaver.h"

#include "Core/Image/TImage.h"
#include "FileIO/Path.h"
#include "FileIO/ImageUtility.h"
#include "ThirdParty/TP_stb-write.h"

#include <cstdio>

namespace cadise
{

void PictureSaver::save(const Path& path, const HDRImage& hdrImage)
{
    if (path.isExtendedWith(".png") || path.isExtendedWith(".PNG") ||
        path.isExtendedWith(".jpg") || path.isExtendedWith(".JPG") ||
        path.isExtendedWith(".ppm") || path.isExtendedWith(".PPM"))
    {
        LDRImage ldrImage;
        ImageUtility::hdrToLdr(hdrImage, &ldrImage);

        _saveLdrImage(path, ldrImage);
    }
    else if (path.isExtendedWith(".hdr") || path.isExtendedWith(".HDR"))
    {
        _saveHdrImage(path, hdrImage);
    }
    else
    {
        // Unsupported image type
    }
}

void PictureSaver::_saveLdrImage(const Path& path, const LDRImage& ldrImage)
{
    if (path.isExtendedWith(".png") || path.isExtendedWith(".PNG"))
    {
        if (!_savePNG(path, ldrImage))
        {

        }
    }
    else if (path.isExtendedWith(".jpg") || path.isExtendedWith(".JPG") ||
        path.isExtendedWith(".jpeg") || path.isExtendedWith(".JPEG"))
    {
        if (!_saveJPG(path, ldrImage))
        {

        }
    }
    else if (path.isExtendedWith(".ppm") || path.isExtendedWith(".PPM"))
    {
        if (!_savePPM(path, ldrImage))
        {

        }
    }
}

void PictureSaver::_saveHdrImage(const Path& path, const HDRImage& hdrImage)
{
    // TODO: hdr saver
}

bool PictureSaver::_savePNG(const Path& path, const LDRImage& ldrImage)
{
    const int32 result = stbi_write_png(
        path.path().c_str(),
        ldrImage.width(),
        ldrImage.height(),
        3,
        ldrImage.rawData(),
        0);

    return result != 0;
}

bool PictureSaver::_saveJPG(const Path& path, const LDRImage& ldrImage)
{
    const int32 result = stbi_write_jpg(
        path.path().c_str(),
        ldrImage.width(),
        ldrImage.height(),
        3,
        ldrImage.rawData(),
        92);

    return result != 0;
}

bool PictureSaver::_savePPM(const Path& path, const LDRImage& ldrImage)
{
    // TODO: Remove .ppm support or update to C++ library rather than C's.

    FILE* output;
    output = fopen(path.path().c_str(), "wb");
    fprintf(output, "P6 %d %d 255\n", ldrImage.width(), ldrImage.height());
    fwrite(ldrImage.rawData(), 1, 3 * ldrImage.width() * ldrImage.height(), output);
    fclose(output);

    return true;
}

} // namespace cadise