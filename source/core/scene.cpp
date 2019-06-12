#include "core/scene.h"

#include "core/camera/camera.h"
#include "core/intersector/accelerator/accelerator.h"
#include "core/light/light.h"

namespace cadise {

Scene::Scene(const std::shared_ptr<Accelerator> accelerator,
             const std::vector<std::shared_ptr<Light> > lights,
             const std::shared_ptr<Camera> camera) :
    _accelerator(std::move(accelerator)), _lights(std::move(lights)), _camera(std::move(camera)) {
}

bool Scene::isIntersecting(Ray &ray, Intersection &intersection) const {
    return _accelerator->isIntersecting(ray, intersection);
}

bool Scene::isOccluded(Ray &ray) const {
    return _accelerator->isOccluded(ray);
}

std::vector<std::shared_ptr<Light> > Scene::lights() const {
    return _lights;
}

std::shared_ptr<Camera> Scene::camera() const {
    return _camera;
}

} // namespace cadise