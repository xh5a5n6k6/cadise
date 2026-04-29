#pragma once

#include "Core/Film/RGBRadianceSensor.h"
#include "Core/Spectrum/Spectrum.h"
#include "FileIO/Path.h"
#include "Math/TVector2.h"
#include "Math/TVector3.h"

#include <memory>
#include <mutex>
#include <vector>

// forward declaration
namespace cadise
{
    class ConnectEvent;
    class FilmTile;
    class Filter;
}

namespace cadise
{

class Film
{
public:
    Film(
        const Vector2I&                resolution,
        const Vector2I&                tileSize,
        const Path&                    filename,
        const std::shared_ptr<Filter>& filter);

    std::unique_ptr<Film> generateEmptyFilm() const;
    void mergeWithFilm(std::unique_ptr<Film> other);
    void replaceWithFilm(std::unique_ptr<Film> other);

    std::unique_ptr<FilmTile> generateFilmTile(const std::size_t tileIndex) const;
    void mergeWithFilmTile(std::unique_ptr<FilmTile> filmTile);

    void addSampleRadiance(const Vector2D& filmPosition, const Spectrum& radiance);
    void addSplatRadiance(const ConnectEvent& connectEvent);

    void save(
        const std::size_t samplesPerPixel,
        const bool        usePostProcessing = true);

    Vector2S numTilesXy() const;
    AABB2I getTileBound(const std::size_t tileIndex) const;

    const Path& filename() const;
    const Vector2I& resolution() const;
    const Vector2I& tileSize() const;

private:
    Vector2I _getTileIndicesXy(const std::size_t tileIndex) const;
    std::size_t _pixelIndexOffset(const int32 x, const int32 y) const;

    Path                    _filename;
    Vector2I                _resolution;
    Vector2I                _tileSize;
    std::shared_ptr<Filter> _filter;

    std::vector<RGBRadianceSensor> _sensorPixels;
    std::vector<Vector3R>          _splatPixels;

    std::mutex _filmMutex;
};

} // namespace cadise