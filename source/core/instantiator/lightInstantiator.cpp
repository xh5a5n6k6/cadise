#include "core/instantiator/instantiator.h"

// light type
#include "core/light/category/environmentLight.h"
#include "core/light/category/pointLight.h"
#include "core/light/category/singleAreaLight.h"

#include "core/imaging/image.h"
#include "core/intersector/primitive/infiniteSphere.h"
#include "core/intersector/primitive/primitive.h"
#include "core/texture/category/rgbImageTexture.h"
#include "file-io/path.h"
#include "file-io/pictureLoader.h"
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

static std::shared_ptr<Light> createSingleArea(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives) {

    const Vector3R color          = data->findVector3r("color");
    const real     watt           = data->findReal("watt");
    const bool     isBackFaceEmit = data->findBool("is-back-face-emit");
    const std::string_view primitiveName = data->findString("primitive");
    auto&& primitive = primitives.find(primitiveName);

    CADISE_ASSERT_NE(primitive, primitives.end());

    std::shared_ptr<SingleAreaLight> areaLight = std::make_shared<SingleAreaLight>(
                                                     primitive->second.get(), color, watt, isBackFaceEmit);

    primitive->second->setAreaLight(areaLight.get());

    return areaLight;
}

static std::shared_ptr<Light> createEnvironment(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives,
    std::shared_ptr<Primitive>& out_backgroundSphere) {
    
    const std::string_view hdrFilename = data->findString("hdr-filename");

    CADISE_ASSERT_NE(hdrFilename, "");

    HdrImage hdrImage = PictureLoader::loadRgbImage(Path(hdrFilename));
    hdrImage.flipHorizontal();

    const TextureSampleMode sampleMode = TextureSampleMode::NEAREST;
    const TextureWrapMode   wrapMode   = TextureWrapMode::REPEAT;
    const std::shared_ptr<Texture<Spectrum>> radiance
        = std::make_shared<RgbImageTexture>(hdrImage, sampleMode, wrapMode);

    out_backgroundSphere = std::make_shared<InfiniteSphere>();
    std::shared_ptr<EnvironmentLight> environmentLight
        = std::make_shared<EnvironmentLight>(out_backgroundSphere.get(), 
                                             radiance, 
                                             hdrImage.resolution());

    out_backgroundSphere->setAreaLight(environmentLight.get());

    return environmentLight;
}

std::shared_ptr<Light> makeLight(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives,
    std::shared_ptr<Primitive>& out_backgroundSphere) {

    CADISE_ASSERT(data);

    std::shared_ptr<Light> light = nullptr;
    const std::string_view type = data->findString("type");
    if (type == "point") {
        light = createPoint(data, primitives);
    }
    else if (type == "area") {
        light = createSingleArea(data, primitives);
    }
    else if (type == "environment") {
        light = createEnvironment(data, primitives, out_backgroundSphere);
    }
    else {
        // unsupported light type
    }

    return light;
}

} // namespace instantiator

} // namespace cadise