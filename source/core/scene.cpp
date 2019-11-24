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
    _topAccelerator(std::move(topAccelerator)),
    _lights(std::move(lights)),
    _environmentSphere(nullptr) {
}

bool Scene::isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const {
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
    else if (_environmentSphere && 
             _environmentSphere->isIntersecting(ray, primitiveInfo)) {

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
    if (_topAccelerator->isOccluded(ray)) {
        return true;
    }
    else if (_environmentSphere && _environmentSphere->isOccluded(ray)) {
        return true;
    }

    return false;
}

const std::vector<std::shared_ptr<Light>>& Scene::lights() const {
    return _lights;
}

void Scene::setEnvironmentSphere(const Primitive* const environmentSphere) {
    CADISE_ASSERT(environmentSphere);

    _environmentSphere = environmentSphere;
}

} // namespace cadise