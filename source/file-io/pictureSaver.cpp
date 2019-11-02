#include "file-io/pictureSaver.h"

#include "core/imaging/image.h"

#include "file-io/path.h"

#include "third-party/tp-stb-write.h"

#include "utility/imageUtils.h"

#include <cstdio>

namespace cadise {

void PictureSaver::save(const Path& path, const HdrImage& hdrImage) {
    if (path.isExtendedWith(".png") || path.isExtendedWith(".PNG") ||
        path.isExtendedWith(".jpg") || path.isExtendedWith(".JPG") ||
        path.isExtendedWith(".ppm") || path.isExtendedWith(".PPM")) {

        LdrImage ldrImage;
        utility::image::hdrToLdr(hdrImage, &ldrImage);
        saveLdrImage(path, ldrImage);
    }
    else if (path.isExtendedWith(".hdr") || path.isExtendedWith(".HDR")) {
        saveHdrImage(path, hdrImage);
    }
    else {
        // Unsupported image type
    }
}

void PictureSaver::saveLdrImage(const Path& path, const LdrImage& ldrImage) {
    if (path.isExtendedWith(".png") || path.isExtendedWith(".PNG")) {
        if (!savePNG(path, ldrImage)) {

        }
    }
    else if (path.isExtendedWith(".jpg") || path.isExtendedWith(".JPG") ||
             path.isExtendedWith(".jpeg") || path.isExtendedWith(".JPEG")) {
        if (!saveJPG(path, ldrImage)) {

        }
    }
    else if (path.isExtendedWith(".ppm") || path.isExtendedWith(".PPM")) {
        if (!savePPM(path, ldrImage)) {

        }
    }
}

void PictureSaver::saveHdrImage(const Path& path, const HdrImage& hdrImage) {
    // TODO: hdr saver
}

bool PictureSaver::savePNG(const Path& path, const LdrImage& ldrImage) {
    int32 result = stbi_write_png(path.path().c_str(), 
                                  ldrImage.width(), 
                                  ldrImage.height(),
                                  3,
                                  ldrImage.rawData(),
                                  0);

    return result != 0;
}

bool PictureSaver::saveJPG(const Path& path, const LdrImage& ldrImage) {
    int32 result = stbi_write_jpg(path.path().c_str(), 
                                  ldrImage.width(), 
                                  ldrImage.height(), 
                                  3, 
                                  ldrImage.rawData(), 
                                  92);

    return result != 0;
}

bool PictureSaver::savePPM(const Path& path, const LdrImage& ldrImage) {
    FILE *output;
    output = fopen(path.path().c_str(), "wb");
    fprintf(output, "P6 %d %d 255\n", ldrImage.width(), ldrImage.height());
    fwrite(ldrImage.rawData(), 1, 3 * ldrImage.width() * ldrImage.height(), output);
    fclose(output);

    return true;
}

} // namespace cadise