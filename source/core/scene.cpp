#include "core/scene.h"

#include "core/intersector/accelerator/accelerator.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/surfaceIntersection.h"

namespace cadise {

Scene::Scene(const std::shared_ptr<Accelerator>& accelerator,
             const std::vector<std::shared_ptr<Light>>& lights) :
    _topAccelerator(std::move(accelerator)),
    _lights(std::move(lights)) {
}

bool Scene::isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const {
    PrimitiveInfo primitiveInfo;
    const bool isHit = _topAccelerator->isIntersecting(ray, primitiveInfo);
    if (isHit) {
        surfaceIntersection.setWi(ray.direction().composite());
        surfaceIntersection.setPrimitiveInfo(primitiveInfo);

        // calculate intersection surface details
        SurfaceInfo surfaceInfo;
        surfaceInfo.setPoint(ray.at(ray.maxT()));

        const Primitive* hitPrimitive = surfaceIntersection.primitiveInfo().primitive();
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, surfaceInfo);
        surfaceIntersection.setSurfaceInfo(surfaceInfo);
    }
    
    return isHit;
}

bool Scene::isOccluded(const Ray& ray) const {
    return _topAccelerator->isOccluded(ray);
}

std::vector<std::shared_ptr<Light>> Scene::lights() const {
    return _lights;
}

} // namespace cadise