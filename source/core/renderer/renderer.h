#pragma once

#include <memory>

namespace cadise {

class Camera;
class Film;
class Scene;

class Renderer {
public:
    Renderer();

    virtual ~Renderer();

    virtual void render() const = 0;
    
    void setScene(const Scene* const scene);
    void setCamera(const std::shared_ptr<Camera>& camera);
    void setFilm(const std::shared_ptr<Film>& film);

protected:
    const Scene*            _scene;
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Film>   _film;

    std::size_t _numWorkers;
};

} // namespace cadise