#include "core/renderer/render-work/tileWork.h"

#include "fundamental/assertion.h"

namespace cadise 
{

TileWork::TileWork() :
    RenderWork(),
    _filmTile(nullptr)
{}

void TileWork::setFilmTile(FilmTile* const filmTile)
{
    CADISE_ASSERT(filmTile);
    
    _filmTile = filmTile;
}

} // namespace cadise