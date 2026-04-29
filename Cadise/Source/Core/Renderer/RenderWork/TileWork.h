#pragma once

#include "Core/Renderer/RenderWork.h"

// forward declaration
namespace cadise
{
    class FilmTile;
}

namespace cadise
{

class TileWork : public RenderWork
{
public:
    TileWork();

    void work() const override = 0;

    void setFilmTile(FilmTile* const filmTile);

protected:
    FilmTile* _filmTile;
};

} // namespace cadise