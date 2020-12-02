#include "core/film/film.h"

#include "core/film/filmTile.h"
#include "core/film/filter/filter.h"
#include "core/imaging/tImage.h"
#include "core/integral-tool/connectEvent.h"
#include "file-io/pictureSaver.h"
#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/type/imageType.h"

#include <cmath>

namespace cadise {

Film::Film(
    const Vector2I&                resolution,
    const Vector2I&                tileSize,
    const Path&                    filename,
    const std::shared_ptr<Filter>& filter) :
    
    _resolution(resolution),
    _tileSize(tileSize),
    _filename(filename),
    _filter(filter),
    _pixels(),
    _splatPixels() {

    CADISE_ASSERT(filter);

    const std::size_t numPixels = _resolution.asType<std::size_t>().product();

    _pixels.resize(numPixels);
    _splatPixels.resize(numPixels);
}

std::unique_ptr<FilmTile> Film::generateFilmTile(const std::size_t tileIndex) const {
    CADISE_ASSERT_LT(tileIndex, this->numTilesXy().product());

    const auto tileIndicesXy = _getTileXyIndices(tileIndex);
    
    return this->generateFilmTile(tileIndicesXy);
}

std::unique_ptr<FilmTile> Film::generateFilmTile(const Vector2I& tileIndicesXy) const {
    const Vector2I minIndicesXy = tileIndicesXy * _tileSize;
    const Vector2I maxIndicesXy = Vector2I::min(minIndicesXy + _tileSize, _resolution);

    return std::make_unique<FilmTile>(
        AABB2I(minIndicesXy, maxIndicesXy),
        _filter.get());

    /*
        origin source code calculates x/y index respectively

        const int32 minIndexX = tileX * _tileSize.x();
        const int32 minIndexY = tileY * _tileSize.y();
        const int32 maxIndexX = math::min(minIndexX + _tileSize.x(), _resolution.x());
        const int32 maxIndexY = math::min(minIndexY + _tileSize.y(), _resolution.y());

        const Vector2I minBound = Vector2I(tileX, tileY) * _tileSize;
        const Vector2I maxBound = Vector2I::min(minBound + _tileSize, _resolution); 

        return std::make_unique<FilmTile>(
            AABB2I({minIndexX, minIndexY}, {maxIndexX, maxIndexY}),
            _filter.get());
    */
}

void Film::mergeWithFilmTile(std::unique_ptr<FilmTile> filmTile) {
    std::lock_guard<std::mutex> lock(_filmMutex);

    const AABB2I& tileBound = filmTile->tileBound();

    const Vector2I& x0y0 = tileBound.minVertex();
    const Vector2I& x1y1 = tileBound.maxVertex();

    // add each pixel value recording in filmTile
    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
            const FilmSensor& sensor 
                = filmTile->getSensor(ix - x0y0.x(), iy - x0y0.y());

            const std::size_t pixelIndexOffset = _pixelIndexOffset(ix, iy);
            const Vector3R totalRadiance(sensor.r(), sensor.g(), sensor.b());
            const real inverseWeight = (sensor.weight() > 0.0_r) ? 1.0_r / sensor.weight() : 1.0_r;

            _pixels[pixelIndexOffset] = totalRadiance * inverseWeight;
        }
    }
}

Vector2S Film::numTilesXy() const {
    return {
        static_cast<std::size_t>((_resolution.x() + _tileSize.x() - 1) / _tileSize.x()), // number of width tiles
        static_cast<std::size_t>((_resolution.y() + _tileSize.y() - 1) / _tileSize.y())  // number of height tiles
    };
}

void Film::addSplatRadiance(const ConnectEvent& connectEvent) {
    std::lock_guard<std::mutex> lock(_filmMutex);

    const Spectrum& splatRadiance = connectEvent.splatRadiance();
    Vector3R splatRgb;
    splatRadiance.transformToRgb(&splatRgb);

    const Vector2R& filmNdcPosition = connectEvent.filmNdcPosition();
    const Vector2R  filmPosition    = filmNdcPosition * _resolution.asType<real>();

    int32 ix = static_cast<int32>(std::floor(filmPosition.x()));
    int32 iy = static_cast<int32>(std::floor(filmPosition.y()));
    ix = (ix < _resolution.x()) ? ix : _resolution.x() - 1;
    iy = (iy < _resolution.y()) ? iy : _resolution.y() - 1;

    const std::size_t pixelIndexOffset = _pixelIndexOffset(ix, iy);
    _splatPixels[pixelIndexOffset] += splatRgb;
}

void Film::save(const std::size_t samplesPerPixel) {
    // TODO: refactor here
    HdrImage hdrImage(_resolution);
    const real inverseSpp = 1.0_r / static_cast<real>(samplesPerPixel);

    for (int32 iy = 0; iy < _resolution.y(); ++iy) {
        for (int32 ix = 0; ix < _resolution.x(); ++ix) {
            const std::size_t pixelOffset = _pixelIndexOffset(ix, iy);

            const FilmPixel& pixel      = _pixels[pixelOffset];
            const FilmPixel& splatPixel = _splatPixels[pixelOffset];

            // TODO: refactor here, use toneMapper instead
            const real r = math::forward_gamma_correction(pixel.x() + splatPixel.x() * inverseSpp);
            const real g = math::forward_gamma_correction(pixel.y() + splatPixel.y() * inverseSpp);
            const real b = math::forward_gamma_correction(pixel.z() + splatPixel.z() * inverseSpp);
            
            hdrImage.setPixelValue(ix, iy, {r, g, b});
        }
    }

    PictureSaver::save(_filename, hdrImage);
}

const Vector2I& Film::resolution() const {
    return _resolution;
}

const Path& Film::filename() const {
    return _filename;
}

Vector2I Film::_getTileXyIndices(const std::size_t tileIndex) const {
    const std::size_t numTilesX = this->numTilesXy().x();

    return {
        static_cast<int32>(tileIndex % numTilesX), // tile x index
        static_cast<int32>(tileIndex / numTilesX)  // tile y index
    };
}

std::size_t Film::_pixelIndexOffset(const int32 x, const int32 y) const {
    return static_cast<std::size_t>(x + y * _resolution.x());
}

} // namespace cadise