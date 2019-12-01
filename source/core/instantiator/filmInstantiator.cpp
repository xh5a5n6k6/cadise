#include "core/instantiator/instantiator.h"

// film type
#include "core/film/film.h"

#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

std::shared_ptr<Film> makeFilm(
    const std::shared_ptr<SdData>& data) {

    CADISE_ASSERT(data);

    std::shared_ptr<Film> film = nullptr;

    const int32            width  = data->findInt32("image-width", 1024);
    const int32            height = data->findInt32("image-height", 768);
    const std::string_view output = data->findString("output-filename", "cadise.jpg");

    const std::shared_ptr<Filter> filter = makeFilter(data);

    return std::make_shared<Film>(width, height, Path(output), filter);
}

} // namespace instantiator

} // namespace cadise