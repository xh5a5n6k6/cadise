#pragma once

#include <memory>
#include <vector>

namespace cadise {

class Accelerator;
class Camera;
class Intersection;
class Light;
class Ray;

class Scene {
public:
    Scene(const std::shared_ptr<Accelerator> accelerator,
          const std::vector<std::shared_ptr<Light> > lights,
          const std::shared_ptr<Camera> camera);

    bool isIntersecting(Ray &ray, Intersection &intersection) const;
    bool isOccluded(Ray &ray) const;
    
    std::vector<std::shared_ptr<Light> > lights() const;
    std::shared_ptr<Camera> camera() const;

private:
    std::shared_ptr<Accelerator> _accelerator;
    std::vector<std::shared_ptr<Light> > _lights;
    std::shared_ptr<Camera> _camera;
};

} // namespace cadise