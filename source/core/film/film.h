#pragma once

#include "core/spectrum/spectrum.h"
#include "file-io/path.h"

#include <memory>
#include <mutex>
#include <vector>

namespace cadise {

class ConnectEvent;
class FilmTile;
class Filter;

class Film {
private:
    using FilmPixel = Vector3R;

public:
    Film(
        const int32                    widthPx, 
        const int32                    heightPx,
        const Path&                    filename,
        const std::shared_ptr<Filter>& filter);

    // TODO: add these two method ?
    //std::unique_ptr<Film> cloneEmpty() const;
    //void mergeWithFilm(std::unique_ptr<Film> other);

    void addSplatRadiance(const ConnectEvent& connectEvent);

    std::unique_ptr<FilmTile> generateFilmTile(const int32 tileX, const int32 tileY) const;
    void mergeWithFilmTile(std::unique_ptr<FilmTile> filmTile);

    void save(const std::size_t samplesPerPixel);

    const Vector2I& resolution() const;

private:
    std::size_t _pixelIndexOffset(const int32 x, const int32 y) const;

    Vector2I                _resolution;
    Path                    _filename;
    std::shared_ptr<Filter> _filter;

    std::vector<FilmPixel> _pixels;
    std::vector<FilmPixel> _splatPixels;

    std::mutex _filmMutex;
};

} // namespace cadise