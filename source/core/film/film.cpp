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
    _sensorPixels(),
    _splatPixels() {

    CADISE_ASSERT(filter);

    const std::size_t numPixels = _resolution.asType<std::size_t>().product();

    _sensorPixels.resize(numPixels);
    _splatPixels.resize(numPixels);

    for (std::size_t i = 0; i < numPixels; ++i) {
        _sensorPixels[i] = RgbRadianceSensor();
        _splatPixels[i]  = Vector3R(0.0_r);
    }
}

std::unique_ptr<Film> Film::generateEmptyFilm() const {
    return std::make_unique<Film>(_resolution, _tileSize, _filename, _filter);
}

void Film::mergeWithFilm(std::unique_ptr<Film> other) {
    // TODO: use valid film bound rather than the whole film
    for (int32 iy = 0; iy < _resolution.y(); ++iy) {
        for (int32 ix = 0; ix < _resolution.x(); ++ix) {
            const std::size_t        sensorIndex = other->_pixelIndexOffset(ix, iy);
            const RgbRadianceSensor& sensor      = other->_sensorPixels[sensorIndex];

            _sensorPixels[sensorIndex].addValue(sensor.r(), sensor.g(), sensor.b());
            _sensorPixels[sensorIndex].addWeight(sensor.weight());
        }
    }
}

void Film::replaceWithFilm(std::unique_ptr<Film> other) {
    // TODO: use valid film bound rather than the whole film
    for (int32 iy = 0; iy < _resolution.y(); ++iy) {
        for (int32 ix = 0; ix < _resolution.x(); ++ix) {
            const std::size_t        sensorIndex = other->_pixelIndexOffset(ix, iy);
            const RgbRadianceSensor& sensor = other->_sensorPixels[sensorIndex];

            _sensorPixels[sensorIndex] = sensor;
        }
    }
}

std::unique_ptr<FilmTile> Film::generateFilmTile(const std::size_t tileIndex) const {
    CADISE_ASSERT_LT(tileIndex, this->numTilesXy().product());

    const AABB2I tileBound = this->getTileBound(tileIndex);

    return std::make_unique<FilmTile>(tileBound, _filter.get());
}

void Film::mergeWithFilmTile(std::unique_ptr<FilmTile> filmTile) {
    std::lock_guard<std::mutex> lock(_filmMutex);

    const AABB2I& tileBound = filmTile->tileBound();

    const Vector2I& x0y0 = tileBound.minVertex();
    const Vector2I& x1y1 = tileBound.maxVertex();

    // add each pixel value recording in filmTile
    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
            const RgbRadianceSensor& sensor 
                = filmTile->getSensor(ix - x0y0.x(), iy - x0y0.y());

            const std::size_t pixelIndexOffset = _pixelIndexOffset(ix, iy);

            _sensorPixels[pixelIndexOffset].addValue(sensor.r(), sensor.g(), sensor.b());
            _sensorPixels[pixelIndexOffset].addWeight(sensor.weight());
        }
    }
}

void Film::addSampleRadiance(const Vector2D& filmPosition, const Spectrum& radiance) {
    if (radiance.hasNaN() || radiance.hasInfinite()) {
        return;
    }

    Vector3R rgb;
    radiance.transformToRgb(&rgb);

    const Vector2R realFilmPosition = filmPosition.asType<real>();

    // calculate filter bound for given film position
    Vector2R filmMinPosition = realFilmPosition - _filter->filterHalfSize();
    Vector2R filmMaxPosition = realFilmPosition + _filter->filterHalfSize();

    filmMinPosition = Vector2R::max(filmMinPosition, Vector2R(0.0_r));
    filmMaxPosition = Vector2R::min(filmMaxPosition, _resolution.asType<real>());

    const Vector2I x0y0(
        static_cast<int32>(std::ceil(filmMinPosition.x() - 0.5_r)),
        static_cast<int32>(std::ceil(filmMinPosition.y() - 0.5_r)));
    const Vector2I x1y1(
        static_cast<int32>(std::floor(filmMaxPosition.x() - 0.5_r) + 1),
        static_cast<int32>(std::floor(filmMaxPosition.y() - 0.5_r) + 1));

    // for each effective pixel, accumulate its weight
    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
            const std::size_t sensorIndexOffset = _pixelIndexOffset(ix , iy);

            const real x = ix - (realFilmPosition.x() - 0.5_r);
            const real y = iy - (realFilmPosition.y() - 0.5_r);

            const real filterWeight = _filter->evaluate(x, y);

            _sensorPixels[sensorIndexOffset].addValue(rgb * filterWeight);
            _sensorPixels[sensorIndexOffset].addWeight(filterWeight);
        }
    }
}

void Film::addSplatRadiance(const ConnectEvent& connectEvent) {
    std::lock_guard<std::mutex> lock(_filmMutex);

    const Spectrum& splatRadiance = connectEvent.splatRadiance();
    Vector3R splatRgb;
    splatRadiance.transformToRgb(&splatRgb);

    const Vector2D& filmPosition = connectEvent.filmPosition();

    int32 ix = static_cast<int32>(std::floor(filmPosition.x()));
    int32 iy = static_cast<int32>(std::floor(filmPosition.y()));
    ix = (ix < _resolution.x()) ? ix : _resolution.x() - 1;
    iy = (iy < _resolution.y()) ? iy : _resolution.y() - 1;

    const std::size_t pixelIndexOffset = _pixelIndexOffset(ix, iy);
    _splatPixels[pixelIndexOffset] += splatRgb;
}

void Film::save(
    const std::size_t samplesPerPixel,
    const bool        usePostProcessing) {

    // TODO: refactor here
    HdrImage hdrImage(_resolution);
    const real inverseSpp = 1.0_r / static_cast<real>(samplesPerPixel);

    for (int32 iy = 0; iy < _resolution.y(); ++iy) {
        for (int32 ix = 0; ix < _resolution.x(); ++ix) {
            const std::size_t pixelOffset = _pixelIndexOffset(ix, iy);

            const RgbRadianceSensor& sensorPixel = _sensorPixels[pixelOffset];
            const Vector3R&          splatPixel  = _splatPixels[pixelOffset];

            const real inverseWeight 
                = (sensorPixel.weight() == 0.0_r) ? 0.0_r : 1.0_r / sensorPixel.weight();
            const Vector3R pixel 
                = Vector3R(sensorPixel.r(), sensorPixel.g(), sensorPixel.b()) * inverseWeight;

            const Vector3R recordLinearSrgb = pixel + splatPixel * inverseSpp;

            if (usePostProcessing) {
                const Vector3R recordSrgb = {
                    math::forward_gamma_correction(recordLinearSrgb.x()),
                    math::forward_gamma_correction(recordLinearSrgb.y()),
                    math::forward_gamma_correction(recordLinearSrgb.z())};

                hdrImage.setPixelValue(ix, iy, recordSrgb);
            }
            else {
                hdrImage.setPixelValue(ix, iy, recordLinearSrgb);
            }
        }
    }

    PictureSaver::save(_filename, hdrImage);
}

Vector2S Film::numTilesXy() const {
    return {
        static_cast<std::size_t>((_resolution.x() + _tileSize.x() - 1) / _tileSize.x()), // number of width tiles
        static_cast<std::size_t>((_resolution.y() + _tileSize.y() - 1) / _tileSize.y())  // number of height tiles
    };
}

AABB2I Film::getTileBound(const std::size_t tileIndex) const {
    const auto tileIndicesXy = _getTileIndicesXy(tileIndex);
    
    const Vector2I minIndicesXy = tileIndicesXy * _tileSize;
    const Vector2I maxIndicesXy = Vector2I::min(minIndicesXy + _tileSize, _resolution);

    return AABB2I(minIndicesXy, maxIndicesXy);
}

const Path& Film::filename() const {
    return _filename;
}

const Vector2I& Film::resolution() const {
    return _resolution;
}

const Vector2I& Film::tileSize() const {
    return _tileSize;
}

Vector2I Film::_getTileIndicesXy(const std::size_t tileIndex) const {
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