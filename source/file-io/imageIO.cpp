#include "file-io/imageIO.h"

#include "file-io/path.h"

#include "third-party/tp-stb.h"

namespace cadise {

void ImageIO::save(Path path, int width, int height, unsigned char* data) {
    if (path.isExtendedWith(".png")) {
        if (!_savePNG(path, width, height, data)) {

        }
    }
    else if (path.isExtendedWith(".ppm")) {
        if (!_savePPM(path, width, height, data)) {

        }
    }
}

bool ImageIO::_savePNG(Path path, int width, int height, unsigned char* data) {
    int result = stbi_write_png(path.path().c_str(), width, height, 3, data, 0);

    return result != 0;
}

bool ImageIO::_savePPM(Path path, int width, int height, unsigned char* data) {
    FILE *output;
    fopen_s(&output, path.path().c_str(), "wb");
    fprintf(output, "P6 %d %d 255\n", width, height);
    fwrite(data, 1, 3 * width * height, output);
    fclose(output);

    return true;
}

} // namespace cadise