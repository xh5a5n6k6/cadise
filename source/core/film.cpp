#include "core/film.h"

#include "file-io/imageIO.h"

#include <algorithm>
#include <memory>

namespace cadise {

Film::Film(const Path& filename, const Vector2I& resolution) :
    _filename(filename), 
    _resolution(resolution) {

    int32 totalValue = 3 * _resolution.x() * _resolution.y();
    _pixelValue = new real[totalValue];
    for (int32 index = 0; index < totalValue; index++) {
        _pixelValue[index] = 0.0_r;
    }
}

void Film::addSample(const int32 px, const int32 py, const Vector3R& value) {
    int32 offset = 3 * (px + py * _resolution.x());
    _pixelValue[offset + 0] += value.x();
    _pixelValue[offset + 1] += value.y();
    _pixelValue[offset + 2] += value.z();
}

void Film::save() const {
    std::unique_ptr<uint8[]> data(new uint8[3 * _resolution.x() * _resolution.y()]);
    for (int32 iy = 0; iy < _resolution.y(); iy++) {
        for (int32 ix = 0; ix < _resolution.x(); ix++) {
            int32 offset = 3 * (ix + iy * _resolution.x());
            data[offset + 0] = static_cast<uint8>(std::clamp(_pixelValue[offset + 0], 0.0_r, 255.0_r));
            data[offset + 1] = static_cast<uint8>(std::clamp(_pixelValue[offset + 1], 0.0_r, 255.0_r));
            data[offset + 2] = static_cast<uint8>(std::clamp(_pixelValue[offset + 2], 0.0_r, 255.0_r));
        }
    }

    imageIO::save(_filename, _resolution.x(), _resolution.y(), data.get());
    data.reset();
    delete[] _pixelValue;
}

Vector2I Film::resolution() const {
    return _resolution;
}

} // namespace cadise