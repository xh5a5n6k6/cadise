#include "core/scene.h"

#include "core/camera/camera.h"
#include "core/intersector/accelerator/accelerator.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/surfaceIntersection.h"

namespace cadise {

Scene::Scene(const std::shared_ptr<Accelerator>& accelerator,
             const std::vector<std::shared_ptr<Light>>& lights,
             const std::shared_ptr<Camera>& camera) :
    _accelerator(std::move(accelerator)),
    _lights(std::move(lights)), 
    _camera(std::move(camera)) {
}

bool Scene::isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const {
    PrimitiveInfo primitiveInfo;
    bool result = _accelerator->isIntersecting(ray, primitiveInfo);
    if (result) {
        surfaceIntersection.setWi(ray.direction().composite());
        surfaceIntersection.setPrimitiveInfo(primitiveInfo);

        // calculate intersection surface details
        SurfaceInfo surfaceInfo;
        surfaceInfo.setPoint(ray.at(ray.maxT()));
        const Primitive* hitPrimitive = surfaceIntersection.primitiveInfo().primitive();
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, surfaceInfo);
        surfaceIntersection.setSurfaceInfo(surfaceInfo);
    }
    
    return result;
}

bool Scene::isOccluded(Ray& ray) const {
    return _accelerator->isOccluded(ray);
}

std::vector<std::shared_ptr<Light>> Scene::lights() const {
    return _lights;
}

std::shared_ptr<Camera> Scene::camera() const {
    return _camera;
}

} // namespace cadise