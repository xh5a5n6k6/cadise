#include "Core/Renderer/RenderWork/TileWork.h"

#include "Foundation/Assertion.h"

namespace cadise
{

TileWork::TileWork() :
    RenderWork(),
    _filmTile(nullptr)
{}

void TileWork::setFilmTile(FilmTile* const filmTile)
{
    CS_ASSERT(filmTile);

    _filmTile = filmTile;
}

} // namespace cadise