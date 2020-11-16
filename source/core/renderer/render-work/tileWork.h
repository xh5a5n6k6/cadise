#pragma once

#include "core/renderer/renderWork.h"

#include "math/vector.h"

namespace cadise {

class FilmTile;

class TileWork : public RenderWork {
public:
    TileWork();

    void work() const override = 0;

    void setFilmTile(FilmTile* const filmTile);
    void setFilmResolution(const Vector2I& filmResolution);

protected:
    FilmTile* _filmTile;
    Vector2I  _filmResolution;
};

} // namespace cadise