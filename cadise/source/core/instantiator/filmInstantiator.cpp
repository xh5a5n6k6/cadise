#include "core/instantiator/instantiator.h"

// film type
#include "core/film/film.h"

#include "file-io/scene-description/CSDResource.h"
#include "fundamental/assertion.h"

namespace cadise::instantiator 
{

std::shared_ptr<Film> makeFilm(
    const std::shared_ptr<CSDResource>& data)
{
    CS_ASSERT(data);

    std::shared_ptr<Film> film = nullptr;

    const int32 width     = data->findInt<int32>("image-width", 1024);
    const int32 height    = data->findInt<int32>("image-height", 768);
    const int32 tileSizeX = data->findInt<int32>("tile-size-x", 16);
    const int32 tileSizeY = data->findInt<int32>("tile-size-y", 16);
    const auto  output    = data->findString("output-filename", "cadise.jpg");

    const auto filter = makeFilter(data);

    return std::make_shared<Film>(
        Vector2I(width, height), 
        Vector2I(tileSizeX, tileSizeY),
        Path(output), 
        filter);
}

} // namespace cadise::instantiator