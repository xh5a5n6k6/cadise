#include "core/renderer/renderer.h"

#include "core/camera/camera.h"
#include "core/film/film.h"

namespace cadise {

Renderer::Renderer() :
    _camera(nullptr),
    _film(nullptr) {
}

void Renderer::setCamera(const std::shared_ptr<Camera>& camera) {
    _camera = camera;
}

void Renderer::setFilm(const std::shared_ptr<Film>& film) {
    _film = film;
}

} // namespace cadise