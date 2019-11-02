#include "core/film/filmTile.h"

#include "core/film/filter/filter.h"

#include "fundamental/assertion.h"

#include "math/vector.h"

namespace cadise {

FilmTile::FilmTile(const AABB2I& tileBound, const Filter* const filter) :
    _resolution(tileBound.extent()),
    _tileBound(tileBound),
    _filter(filter),
    _sensors() {

    const std::size_t sensorSize = static_cast<std::size_t>(_resolution.x() * _resolution.y());
    setSensorSize(sensorSize);
}

void FilmTile::addSample(const Vector2R& filmPosition, const Spectrum& sampleSpectrum) {
    // TODO: do this check
    //CADISE_ASSERT(sampleSpectrum.hasNaN());
    //CADISE_ASSERT(sampleSpectrum.hasInfinite());

    if (sampleSpectrum.hasNaN() || sampleSpectrum.hasInfinite()) {
        return;
    }

    Vector3R rgb;
    sampleSpectrum.transformToRgb(&rgb);
    addSample(filmPosition, rgb);
}

void FilmTile::addSample(const Vector2R& filmPosition, const Vector3R& sampleRgb) {
    // calculate filter bound for given film position
    Vector2R filmMinPosition = filmPosition - _filter->filterHalfSize();
    Vector2R filmMaxPosition = filmPosition + _filter->filterHalfSize();

    filmMinPosition = Vector2R::max(filmMinPosition, Vector2R(_tileBound.minVertex()));
    filmMaxPosition = Vector2R::min(filmMaxPosition, Vector2R(_tileBound.maxVertex()));

    const Vector2I x0y0(static_cast<int32>(std::ceil(filmMinPosition.x() - 0.5_r)),
                        static_cast<int32>(std::ceil(filmMinPosition.y() - 0.5_r)));
    const Vector2I x1y1(static_cast<int32>(std::floor(filmMaxPosition.x() - 0.5_r) + 1),
                        static_cast<int32>(std::floor(filmMaxPosition.y() - 0.5_r) + 1));

    // for each effective pixel, accumulate its weight
    for (int32 iy = x0y0.y(); iy < x1y1.y(); ++iy) {
        for (int32 ix = x0y0.x(); ix < x1y1.x(); ++ix) {
            const std::size_t sensorIndexOffset = _sensorIndexOffset(ix - _tileBound.minVertex().x(), 
                                                                     iy - _tileBound.minVertex().y());

            const real x = ix - (filmPosition.x() - 0.5_r);
            const real y = iy - (filmPosition.y() - 0.5_r);

            const real filterWeight = _filter->evaluate(x, y);

            _sensors[sensorIndexOffset].addValue(sampleRgb * filterWeight);
            _sensors[sensorIndexOffset].addWeight(filterWeight);
        }
    }
}

AABB2I FilmTile::tileBound() const {
    return _tileBound;
}

const FilmSensor& FilmTile::getSensor(const int32 x, const int32 y) const {
    const std::size_t sensorIndexOffset = _sensorIndexOffset(x, y);
    return _sensors[sensorIndexOffset];
}

void FilmTile::setSensorSize(const std::size_t sensorSize) {
    _sensors.resize(sensorSize);
}

std::size_t FilmTile::_sensorIndexOffset(const int32 x, const int32 y) const {
    return static_cast<std::size_t>(x + y * _resolution.x());
}

} // namespace cadise