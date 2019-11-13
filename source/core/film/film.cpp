#include "core/film/film.h"

#include "core/film/filmTile.h"
#include "core/film/filter/filter.h"
#include "core/imaging/image.h"

#include "file-io/pictureSaver.h"

#include "fundamental/assertion.h"

#include "math/math.h"
#include "math/type/imageType.h"

#include <cmath>

namespace cadise {

Film::Film(const int32 widthPx, 
           const int32 heightPx, 
           const Path& filename,
           const std::shared_ptr<Filter>& filter) :
    _resolution(widthPx, heightPx),
    _filename(filename),
    _filter(filter),
    _pixels() {

    const std::size_t pixelNumber = static_cast<std::size_t>(_resolution.x() * _resolution.y());
    _pixels.resize(pixelNumber);
}

std::unique_ptr<FilmTile> Film::generateFilmTile(const int32 tileX, const int32 tileY) const {
    const int32 minIndexX = tileX * CADISE_FILMTILE_SIZE;
    const int32 minIndexY = tileY * CADISE_FILMTILE_SIZE;

    const int32 maxIndexX = math::min(minIndexX + CADISE_FILMTILE_SIZE, _resolution.x());
    const int32 maxIndexY = math::min(minIndexY + CADISE_FILMTILE_SIZE, _resolution.y());

    return std::make_unique<FilmTile>(AABB2I({minIndexX, minIndexY},
                                             {maxIndexX, maxIndexY}),
                                      _filter.get());
}

void Film::mergeWithFilmTile(std::unique_ptr<FilmTile> filmTile) {
    const AABB2I tileBound = filmTile->tileBound();

    const Vector2I x0y0 = tileBound.minVertex();
    const Vector2I x1y1 = tileBound.maxVertex();

    // add each pixel value recording in filmTile
    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
            const FilmSensor sensor = filmTile->getSensor(ix - x0y0.x(),
                                                          iy - x0y0.y());
            const std::size_t pixelIndexOffset = _pixelIndexOffset(ix, iy);
            const Vector3R totalRadiance(sensor.r(), sensor.g(), sensor.b());
            const real inverseWeight = 1.0_r / sensor.weight();

            _pixels[pixelIndexOffset] = totalRadiance * inverseWeight;
        }
    }
}

void Film::save() {
    // TODO: refactor here
    HdrImage hdrImage(_resolution);

    for (int32 iy = 0; iy < _resolution.y(); ++iy) {
        for (int32 ix = 0; ix < _resolution.x(); ++ix) {
            const std::size_t pixelOffset = _pixelIndexOffset(ix, iy);

            const FilmPixel pixel = _pixels[pixelOffset];

            const real r = math::gammaCorrection(pixel.x());
            const real g = math::gammaCorrection(pixel.y());
            const real b = math::gammaCorrection(pixel.z());

            hdrImage.setPixelValue(ix, iy, {r, g, b});
        }
    }

    PictureSaver::save(_filename, hdrImage);
    _pixels.clear();
    _pixels.shrink_to_fit();
}

const Vector2I& Film::resolution() const {
    return _resolution;
}

std::size_t Film::_pixelIndexOffset(const int32 x, const int32 y) const {
    return static_cast<std::size_t>(x + y * _resolution.x());
}

} // namespace cadise