#pragma once

#include "core/renderer/renderWork.h"

#include "math/tVector.h"

namespace cadise {

class FilmTile;

class TileWork : public RenderWork {
public:
    TileWork();

    void work() const override = 0;

    void setFilmTile(FilmTile* const filmTile);

protected:
    FilmTile* _filmTile;
};

} // namespace cadise