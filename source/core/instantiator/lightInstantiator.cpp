#include "core/instantiator/instantiator.h"

// light type
#include "core/light/areaLight.h"
#include "core/light/pointLight.h"

// for area light
#include "core/intersector/primitive/primitive.h"

#include "file-io/scene-description/sdData.h"

#include "fundamental/assertion.h"

namespace cadise {

namespace instantiator {

static std::shared_ptr<Light> createPoint(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives) {

    const Vector3R position = data->findVector3r("position");
    const Vector3R color    = data->findVector3r("color");

    return std::make_shared<PointLight>(position, color);
}

static std::shared_ptr<Light> createArea(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives) {

    const Vector3R color          = data->findVector3r("color");
    const real     watt           = data->findReal("watt");
    const bool     isBackFaceEmit = data->findBool("is-back-face-emit");

    std::shared_ptr<AreaLight> areaLight = std::make_shared<AreaLight>(color, watt, isBackFaceEmit);

    const std::string_view primitiveName = data->findString("primitive");
    auto&& primitive = primitives.find(primitiveName);

    CADISE_ASSERT(primitive != primitives.end());

    areaLight->setPrimitive(primitive->second);
    areaLight->calculateEmitRadiance();
    primitive->second->setAreaLight(areaLight);

    return areaLight;
}

std::shared_ptr<Light> makeLight(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives) {

    std::shared_ptr<Light> light = nullptr;
    const std::string_view type = data->findString("type");
    if (type == "point") {
        light = createPoint(data, primitives);
    }
    else if (type == "area") {
        light = createArea(data, primitives);
    }
    else {
        // don't support light type
    }

    return light;
}

} // namespace instantiator

} // namespace cadise