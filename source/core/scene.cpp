#include "core/scene.h"

namespace cadise {

Scene::Scene() {
}

Scene::Scene(std::vector<std::shared_ptr<Intersector> > intersectors,
             std::vector<std::shared_ptr<Light> > lights,
             std::shared_ptr<Camera> camera) :
    _intersectors(intersectors), _lights(lights), _camera(camera) {
}

bool Scene::isIntersecting(Ray &ray, Intersection &intersection) {
    bool result = false;
    for (int i = 0; i < _intersectors.size(); i++) 	
        result |= _intersectors[i]->isIntersecting(ray, intersection);

    return result;
}

bool Scene::isOccluded(Ray &ray) {
    bool result = false;
    for (int i = 0; i < _intersectors.size(); i++)
        result |= _intersectors[i]->isOccluded(ray);

    return result;
}

} // namespace cadise