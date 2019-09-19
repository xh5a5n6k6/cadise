#pragma once

#include <memory>

namespace cadise {

class Camera;
class Scene;

class Renderer {
public:

    virtual void render(const Scene& scene) const = 0;
    
    void setCamera(const std::shared_ptr<Camera>& camera);

protected:
    std::shared_ptr<Camera> _camera;
};

} // namespace cadise