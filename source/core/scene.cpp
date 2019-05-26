#include "core/scene.h"

#include "core/accelerator/accelerator.h"
#include "core/camera/camera.h"
#include "core/light/light.h"

namespace cadise {

Scene::Scene(std::shared_ptr<Accelerator> accelerator,
             std::vector<std::shared_ptr<Light> > lights,
             std::shared_ptr<Camera> camera) :
    _accelerator(std::move(accelerator)), _lights(std::move(lights)), _camera(std::move(camera)) {
}

bool Scene::isIntersecting(Ray &ray, Intersection &intersection) {
    return _accelerator->isIntersecting(ray, intersection);
}

bool Scene::isOccluded(Ray &ray) {
    return _accelerator->isOccluded(ray);
}

std::vector<std::shared_ptr<Light> > Scene::lights() {
    return _lights;
}

std::shared_ptr<Camera> Scene::camera() {
    return _camera;
}

} // namespace cadise