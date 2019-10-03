#pragma once

#include <memory>

namespace cadise {

class Camera;
class Film;
class Scene;

class Renderer {
public:
    Renderer();

    virtual void render(const Scene& scene) const = 0;
    
    void setCamera(const std::shared_ptr<Camera>& camera);
    void setFilm(const std::shared_ptr<Film>& film);

protected:
    std::shared_ptr<Camera> _camera;
    std::shared_ptr<Film>   _film;
};

} // namespace cadise