#include "core/renderer/renderer.h"

#include "core/camera/camera.h"
#include "core/film/film.h"
#include "fundamental/assertion.h"

namespace cadise {

Renderer::Renderer() :
    _camera(nullptr),
    _film(nullptr) {
}

Renderer::~Renderer() = default;

void Renderer::setCamera(const std::shared_ptr<Camera>& camera) {
    CADISE_ASSERT(camera);

    _camera = camera;
}

void Renderer::setFilm(const std::shared_ptr<Film>& film) {
    CADISE_ASSERT(film);

    _film = film;
}

} // namespace cadise