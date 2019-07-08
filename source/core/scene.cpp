#include "core/scene.h"

#include "core/camera/camera.h"
#include "core/intersector/accelerator/accelerator.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/ray.h"
#include "core/surfaceIntersection.h"

namespace cadise {

Scene::Scene(const std::shared_ptr<Accelerator> accelerator,
             const std::vector<std::shared_ptr<Light> > lights,
             const std::shared_ptr<Camera> camera) :
    _accelerator(std::move(accelerator)), _lights(std::move(lights)), _camera(std::move(camera)) {
}

bool Scene::isIntersecting(Ray &ray, SurfaceIntersection &surfaceIntersection) const {
    bool result = _accelerator->isIntersecting(ray, surfaceIntersection.primitiveInfo());
    if (result) {
        // calculate intersection geometry details
        SurfaceGeometryInfo geometryInfo;
        geometryInfo.setPoint(ray.at(ray.maxT()));
        surfaceIntersection.primitiveInfo().primitive()->evaluateGeometryDetail(surfaceIntersection.primitiveInfo(), geometryInfo);
        surfaceIntersection.setSurfaceGeometryInfo(geometryInfo);

        // calculate intersection shading details
        SurfaceShadingInfo shadingInfo;
        surfaceIntersection.primitiveInfo().primitive()->evaluteShadingDetail(shadingInfo);
    }
    
    return result;
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