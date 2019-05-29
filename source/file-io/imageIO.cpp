#include "file-io/imageIO.h"

#include "file-io/path.h"

#include "third-party/tp-stb.h"

#include <cstdio>

namespace cadise {

namespace imageIO {

static bool savePNG(Path path, int width, int height, uint8* data) {
    int result = stbi_write_png(path.path().c_str(), width, height, 3, data, 0);

    return result != 0;
}

static bool savePPM(Path path, int width, int height, uint8* data) {
    FILE *output;
    output = fopen(path.path().c_str(), "wb");
    fprintf(output, "P6 %d %d 255\n", width, height);
    fwrite(data, 1, 3 * width * height, output);
    fclose(output);

    return true;
}

void save(Path path, int width, int height, uint8* data) {
    if (path.isExtendedWith(".png")) {
        if (!savePNG(path, width, height, data)) {

        }
    }
    else if (path.isExtendedWith(".ppm")) {
        if (!savePPM(path, width, height, data)) {

        }
    }
}

} // namespace imageIO

} // namespace cadise