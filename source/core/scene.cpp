#include "core/scene.h"

#include "core/intersector/accelerator/accelerator.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

namespace cadise {

Scene::Scene(const std::shared_ptr<Accelerator>& topAccelerator,
             const std::vector<std::shared_ptr<Light>>& lights) :
    
    _topAccelerator(topAccelerator),
    _lights(std::move(lights)),
    _backgroundSphere(nullptr) {

    CADISE_ASSERT(topAccelerator);
}

bool Scene::isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const {
    // TODO: Refactor here
    PrimitiveInfo primitiveInfo;
    if(_topAccelerator->isIntersecting(ray, primitiveInfo)) {
        surfaceIntersection.setWi(ray.direction().reverse());
        surfaceIntersection.setPrimitiveInfo(primitiveInfo);

        // calculate intersection surface details
        SurfaceInfo surfaceInfo;
        surfaceInfo.setPoint(ray.at(ray.maxT()));

        const Primitive* hitPrimitive = surfaceIntersection.primitiveInfo().primitive();
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, surfaceInfo);
        surfaceIntersection.setSurfaceInfo(surfaceInfo);

        return true;
    }
    else if (_backgroundSphere && 
             _backgroundSphere->isIntersecting(ray, primitiveInfo)) {

        surfaceIntersection.setWi(ray.direction().reverse());
        surfaceIntersection.setPrimitiveInfo(primitiveInfo);

        // calculate intersection surface details
        SurfaceInfo surfaceInfo;
        surfaceInfo.setPoint(ray.direction());

        const Primitive* hitPrimitive = surfaceIntersection.primitiveInfo().primitive();
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, surfaceInfo);
        surfaceIntersection.setSurfaceInfo(surfaceInfo);

        return true;
    }
    
    return false;
}

bool Scene::isOccluded(const Ray& ray) const {
    // TODO: Refactor here
    if (_topAccelerator->isOccluded(ray)) {
        return true;
    }
    else if (_backgroundSphere && _backgroundSphere->isOccluded(ray)) {
        return true;
    }

    return false;
}

const std::vector<std::shared_ptr<Light>>& Scene::lights() const {
    return _lights;
}

void Scene::setBackgroundSphere(const Primitive* const backgroundSphere) {
    CADISE_ASSERT(backgroundSphere);

    _backgroundSphere = backgroundSphere;
}

} // namespace cadise