#include "core/instantiator/instantiator.h"

// light type
#include "core/light/areaLight.h"
#include "core/light/pointLight.h"

// for area light
#include "core/intersector/primitive/primitive.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Light> createPoint(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Primitive>, std::less<>>& primitives) {

    const Vector3R position = data->findVector3r("position");
    const Vector3R color    = data->findVector3r("color");

    return std::make_shared<PointLight>(position, color);
}

static std::shared_ptr<Light> createArea(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Primitive>, std::less<>>& primitives) {

    const Vector3R albedo         = data->findVector3r("albedo");
    const bool     isBackFaceEmit = data->findBool("is-back-face-emit");

    std::shared_ptr<AreaLight> areaLight = std::make_shared<AreaLight>(albedo, isBackFaceEmit);

    std::string_view primitiveName = data->findString("primitive");
    auto&& primitive = primitives.find(primitiveName);

    areaLight->setPrimitive(primitive->second);
    primitive->second->setAreaLight(areaLight);

    return areaLight;
}

std::shared_ptr<Light> makeLight(
    const std::shared_ptr<SdData>& data,
    const std::map<std::string, std::shared_ptr<Primitive>, std::less<>>& primitives) {

    std::shared_ptr<Light> light = nullptr;
    std::string_view type = data->findString("type");
    if (!type.compare("point")) {
        light = createPoint(data, primitives);
    }
    else if (!type.compare("area")) {
        light = createArea(data, primitives);
    }
    else {
        // don't support light type
    }

    return light;
}

} // namespace instantiator

} // namespace cadise