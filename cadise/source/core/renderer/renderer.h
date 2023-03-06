#pragma once

#include <memory>

// forward declaration
namespace cadise
{
    class Camera;
    class Film;
    class Scene;
}

namespace cadise
{

class Renderer
{
public:
    Renderer();

    virtual ~Renderer();

    virtual void render() const = 0;
    
    void setScene(const std::shared_ptr<Scene>& scene);
    void setCamera(const std::shared_ptr<Camera>& camera);
    void setFilm(const std::shared_ptr<Film>& film);
    void setWorkerCount(const std::size_t workerCount);

protected:
    std::shared_ptr<Scene>  _scene;
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Film>   _film;

    std::size_t _numWorkers;
};

} // namespace cadise