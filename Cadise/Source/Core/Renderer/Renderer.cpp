#include "Core/Renderer/Renderer.h"

#include "Foundation/Assertion.h"

namespace cadise 
{

Renderer::Renderer() :
    _scene(nullptr),
    _camera(nullptr),
    _film(nullptr),
    _numWorkers(1)
{}

Renderer::~Renderer() = default;

void Renderer::setScene(const std::shared_ptr<Scene>& scene)
{
    CS_ASSERT(scene);

    _scene = scene;
}

void Renderer::setCamera(const std::shared_ptr<Camera>& camera) 
{
    CS_ASSERT(camera);

    _camera = camera;
}

void Renderer::setFilm(const std::shared_ptr<Film>& film)
{
    CS_ASSERT(film);

    _film = film;
}

void Renderer::setWorkerCount(const std::size_t workerCount)
{
    _numWorkers = workerCount;
}

} // namespace cadise