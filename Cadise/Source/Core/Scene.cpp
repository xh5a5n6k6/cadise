#include "Core/Scene.h"

#include "Core/Intersector/Accelerator/Accelerator.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Light/Light.h"
#include "Core/Light/Cluster/LightCluster.h"
#include "Core/Ray.h"
#include "Core/SurfaceIntersection.h"
#include "Foundation/Assertion.h"

namespace cadise 
{

Scene::Scene(
    const std::shared_ptr<Accelerator>&  topAccelerator,
    const std::shared_ptr<LightCluster>& lightCluster) :

    _topAccelerator(topAccelerator),
    _lightCluster(lightCluster),
    _backgroundSphere(nullptr) 
{
    CS_ASSERT(topAccelerator);
    CS_ASSERT(lightCluster);
}

void Scene::evaluateBound(AABB3R* const out_bound) const
{
    CS_ASSERT(out_bound);

    _topAccelerator->evaluateBound(out_bound);
}

bool Scene::isIntersecting(Ray& ray, SurfaceIntersection& surfaceIntersection) const
{
    // TODO: Refactor here
    PrimitiveInfo primitiveInfo;
    if(_topAccelerator->isIntersecting(ray, primitiveInfo)) 
    {
        surfaceIntersection.setWi(ray.direction().negate());
        surfaceIntersection.setPrimitiveInfo(primitiveInfo);

        // calculate intersection surface details
        SurfaceDetail surfaceDetail;
        surfaceDetail.setPosition(ray.at(ray.maxT()));

        const Primitive* hitPrimitive = surfaceIntersection.primitiveInfo().primitive();
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, &surfaceDetail);

        surfaceDetail.computeCoordinateSystem();
        surfaceIntersection.setSurfaceDetail(surfaceDetail);

        return true;
    }
    else if (_backgroundSphere && 
             _backgroundSphere->isIntersecting(ray, primitiveInfo))
    {
        surfaceIntersection.setWi(ray.direction().negate());
        surfaceIntersection.setPrimitiveInfo(primitiveInfo);

        // calculate intersection surface details
        SurfaceDetail surfaceDetail;
        surfaceDetail.setPosition(ray.direction());

        const Primitive* hitPrimitive = surfaceIntersection.primitiveInfo().primitive();
        hitPrimitive->evaluateSurfaceDetail(primitiveInfo, &surfaceDetail);

        surfaceDetail.computeCoordinateSystem();
        surfaceIntersection.setSurfaceDetail(surfaceDetail);

        return true;
    }
    
    return false;
}

bool Scene::isOccluded(const Ray& ray) const 
{
    // TODO: Refactor here
    if (_topAccelerator->isOccluded(ray))
    {
        return true;
    }
    else if (_backgroundSphere && _backgroundSphere->isOccluded(ray))
    {
        return true;
    }

    return false;
}

const Light* Scene::sampleOneLight(real* const out_pdf) const 
{
    CS_ASSERT(out_pdf);

    return _lightCluster->sampleOneLight(out_pdf);
}

real Scene::evaluatePickLightPdf(const Light* const light) const
{
    CS_ASSERT(light);

    return _lightCluster->evaluatePickLightPdf(light);
}

void Scene::setBackgroundSphere(const Primitive* const backgroundSphere)
{
    CS_ASSERT(backgroundSphere);

    _backgroundSphere = backgroundSphere;
}

void Scene::unsetBackgroundSphere() 
{
    _backgroundSphere = nullptr;
}

} // namespace cadise