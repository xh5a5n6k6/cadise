#include "core/film.h"

#include "file-io/imageIO.h"

namespace cadise {

Film::Film() :
    _resolution(0) {
}

Film::Film(Path filename, int rx, int ry) :
    _filename(filename), _resolution(rx, ry) {
    _pixelValue = new float[3 * _resolution.x() * _resolution.y()];
}

void Film::addSample(int px, int py, Vector3F value) {
    int offset = 3 * (px + py * _resolution.x());
    _pixelValue[offset]     += value.x();
    _pixelValue[offset + 1] += value.y();
    _pixelValue[offset + 2] += value.z();
}

void Film::save() {
    std::unique_ptr<uint8[]> data(new uint8[3 * _resolution.x() * _resolution.y()]);
    for (int y = 0; y < _resolution.y(); y++) {
        for (int x = 0; x < _resolution.x(); x++) {
            int offset = 3 * (x + y * _resolution.x());
            data[offset]     = static_cast<uint8>(_pixelValue[offset    ]);
            data[offset + 1] = static_cast<uint8>(_pixelValue[offset + 1]);
            data[offset + 2] = static_cast<uint8>(_pixelValue[offset + 2]);
        }
    }

    imageIO::save(_filename, _resolution.x(), _resolution.y(), data.get());
    data.release();
}

Vector2I Film::resolution() {
    return _resolution;
}

} // namespace cadise