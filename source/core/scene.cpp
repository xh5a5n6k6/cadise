#include "core/scene.h"

#include "core/intersector/accelerator/accelerator.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/light/cluster/lightCluster.h"
#include "core/ray.h"
#include "core/surfaceIntersection.h"
#include "fundamental/assertion.h"

namespace cadise {

Scene::Scene(const std::shared_ptr<Accelerator>&  topAccelerator,
             const std::shared_ptr<LightCluster>& lightCluster) :

    _topAccelerator(topAccelerator),
    _lightCluster(lightCluster),
    _backgroundSphere(nullptr) {

    CADISE_ASSERT(topAccelerator);
    CADISE_ASSERT(lightCluster);
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
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, &surfaceInfo);
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
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, &surfaceInfo);
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

const Light* Scene::sampleOneLight(real* const out_pdf) const {
    CADISE_ASSERT(out_pdf);

    return _lightCluster->sampleOneLight(out_pdf);
}

void Scene::setBackgroundSphere(const Primitive* const backgroundSphere) {
    CADISE_ASSERT(backgroundSphere);

    _backgroundSphere = backgroundSphere;
}

} // namespace cadise