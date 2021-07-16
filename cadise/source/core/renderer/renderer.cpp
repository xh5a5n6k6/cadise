#include "core/renderer/renderer.h"

#include "fundamental/assertion.h"

#include <thread>

namespace cadise {

Renderer::Renderer() :
    _scene(nullptr),
    _camera(nullptr),
    _film(nullptr) {

    _numWorkers = static_cast<std::size_t>(std::thread::hardware_concurrency());
}

Renderer::~Renderer() = default;

void Renderer::setScene(const Scene* const scene) {
    CADISE_ASSERT(scene);

    _scene = scene;
}

void Renderer::setCamera(const std::shared_ptr<Camera>& camera) {
    CADISE_ASSERT(camera);

    _camera = camera;
}

void Renderer::setFilm(const std::shared_ptr<Film>& film) {
    CADISE_ASSERT(film);

    _film = film;
}

} // namespace cadise