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
    Scene();
    Scene(std::shared_ptr<Accelerator> accelerator,
          std::vector<std::shared_ptr<Light> > lights, 
          std::shared_ptr<Camera> camera);

    bool isIntersecting(Ray &ray, Intersection &intersection);
    bool isOccluded(Ray &ray);
    
    std::vector<std::shared_ptr<Light> > lights();
    std::shared_ptr<Camera> camera();

private:
    std::shared_ptr<Accelerator> _accelerator;
    std::vector<std::shared_ptr<Light> > _lights;
    std::shared_ptr<Camera> _camera;
};

} // namespace cadise