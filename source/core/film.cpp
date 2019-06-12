#include "core/film.h"

#include "file-io/imageIO.h"

#include <memory>

namespace cadise {

Film::Film(const Path filename, const int32 rx, const int32 ry) :
    _filename(filename), _resolution(rx, ry) {
    _pixelValue = new real[3 * _resolution.x() * _resolution.y()];
}

void Film::addSample(const int32 px, const int32 py, const Vector3R value) {
    int32 offset = 3 * (px + py * _resolution.x());
    _pixelValue[offset]     += value.x();
    _pixelValue[offset + 1] += value.y();
    _pixelValue[offset + 2] += value.z();
}

void Film::save() const {
    std::unique_ptr<uint8[]> data(new uint8[3 * _resolution.x() * _resolution.y()]);
    for (int32 y = 0; y < _resolution.y(); y++) {
        for (int32 x = 0; x < _resolution.x(); x++) {
            int32 offset = 3 * (x + y * _resolution.x());
            data[offset]     = static_cast<uint8>(_pixelValue[offset    ]);
            data[offset + 1] = static_cast<uint8>(_pixelValue[offset + 1]);
            data[offset + 2] = static_cast<uint8>(_pixelValue[offset + 2]);
        }
    }

    imageIO::save(_filename, _resolution.x(), _resolution.y(), data.get());
    data.release();
}

Vector2I Film::resolution() const {
    return _resolution;
}

} // namespace cadise