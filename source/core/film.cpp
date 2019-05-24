#include "core/film.h"

#include "file-io/imageIO.h"

#include "math/vector.h"

namespace cadise {

Film::Film() :
    _resolutionX(0), _resolutionY(0) {
}

Film::Film(Path filename, int rx, int ry) :
    _filename(filename), _resolutionX(rx), _resolutionY(ry) {
    _pixelValue = new float[3 * _resolutionX * _resolutionY];
}

void Film::addSample(int px, int py, Vector3 value) {
    int offset = 3 * (px + py * _resolutionX);
    _pixelValue[offset]     += value.x();
    _pixelValue[offset + 1] += value.y();
    _pixelValue[offset + 2] += value.z();
}

void Film::save() {
    std::unique_ptr<uint8[]> data(new uint8[3 * _resolutionX * _resolutionY]);
    for (int y = 0; y < _resolutionY; y++) {
        for (int x = 0; x < _resolutionX; x++) {
            int offset = 3 * (x + y * _resolutionX);
            data[offset]     = static_cast<uint8>(_pixelValue[offset    ]);
            data[offset + 1] = static_cast<uint8>(_pixelValue[offset + 1]);
            data[offset + 2] = static_cast<uint8>(_pixelValue[offset + 2]);
        }
    }

    imageIO::save(_filename, _resolutionX, _resolutionY, data.get());
    data.release();
}

int Film::resolutionX() {
    return _resolutionX;
}

int Film::resolutionY() {
    return _resolutionY;
}

} // namespace cadise