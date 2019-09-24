#include "core/film/film.h"

#include "core/film/filter/filter.h"
#include "core/film/pixel.h"

#include "file-io/imageIO.h"

#include "math/math.h"

#include <cmath>

namespace cadise {

Film::Film(const int32 widthPx, const int32 heightPx, 
           const Path& filename,
           const std::shared_ptr<Filter>& filter) :
    _filename(filename), 
    _resolution(widthPx, heightPx),
    _filter(filter) {

    const int32 pixelNumber = _resolution.x() * _resolution.y();
    _pixels = new Pixel[pixelNumber];
}

void Film::addSample(const Vector2R& filmPosition, const Spectrum& value) {
    if (value.hasNaN() || value.hasInfinite()) {
        return;
    }

    const Vector3R sampleRgb = value.transformToRgb();

    // calculate filter bound for given film position
    Vector2R filmMinPosition = filmPosition - _filter->filterHalfSize();
    Vector2R filmMaxPosition = filmPosition + _filter->filterHalfSize();

    filmMinPosition = Vector2R::max(filmMinPosition, Vector2R(0.0_r));
    filmMaxPosition = Vector2R::min(filmMaxPosition, Vector2R(_resolution.x() - 1.0_r, _resolution.y() - 1.0_r));

    const int32 xMinIndex = static_cast<int32>(std::ceil(filmMinPosition.x() - 0.5_r));
    const int32 yMinIndex = static_cast<int32>(std::ceil(filmMinPosition.y() - 0.5_r));
    const int32 xMaxIndex = static_cast<int32>(std::floor(filmMaxPosition.x() - 0.5_r));
    const int32 yMaxIndex = static_cast<int32>(std::floor(filmMaxPosition.y() - 0.5_r));

    // for each effective pixel, accumulate its weight
    for (int32 iy = yMinIndex; iy <= yMaxIndex; iy++) {
        for (int32 ix = xMinIndex; ix <= xMaxIndex; ix++) {
            const int32 indexOffset = ix + iy * _resolution.x();
            const real x = ix - (filmPosition.x() - 0.5_r);
            const real y = iy - (filmPosition.y() - 0.5_r);

            const real filterWeight = _filter->evaluate(x, y);

            _pixels[indexOffset].addValue(255.0_r * sampleRgb * filterWeight);
            _pixels[indexOffset].addWeight(filterWeight);
        }
    }
}

void Film::save() const {
    std::unique_ptr<uint8[]> data(new uint8[3 * _resolution.x() * _resolution.y()]);
    for (int32 iy = 0; iy < _resolution.y(); iy++) {
        for (int32 ix = 0; ix < _resolution.x(); ix++) {
            const int32 pixelOffset = ix + iy * _resolution.x();
            const int32 dataOffset  = 3 * pixelOffset;
            const Pixel* pixel = &_pixels[pixelOffset];
            
            data[dataOffset + 0] = static_cast<uint8>(math::clamp(pixel->r() / pixel->weight(), 0.0_r, 255.0_r));
            data[dataOffset + 1] = static_cast<uint8>(math::clamp(pixel->g() / pixel->weight(), 0.0_r, 255.0_r));
            data[dataOffset + 2] = static_cast<uint8>(math::clamp(pixel->b() / pixel->weight(), 0.0_r, 255.0_r));
        }
    }

    imageIO::save(_filename, _resolution.x(), _resolution.y(), data.get());
    data.reset();
    delete[] _pixels;
}

Vector2I Film::resolution() const {
    return _resolution;
}

} // namespace cadise