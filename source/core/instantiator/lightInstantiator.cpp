#include "core/instantiator/instantiator.h"

// light type
#include "core/light/category/environmentLight.h"
#include "core/light/category/pointLight.h"
#include "core/light/category/singleAreaLight.h"

#include "core/imaging/tImage.h"
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

    const auto position  = data->findVector3r("position");
    const auto intensity = data->findVector3r("intensity");

    return std::make_shared<PointLight>(position, Spectrum(intensity));
}

static std::shared_ptr<Light> createSingleArea(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives) {

    const auto color          = data->findVector3r("color");
    const real watt           = data->findReal("watt");
    const bool isBackFaceEmit = data->findBool("is-back-face-emit");

    const auto   primitiveName = data->findString("primitive");
    const auto&& primitive     = primitives.find(primitiveName);

    CADISE_ASSERT_NE(primitive, primitives.end());

    std::shared_ptr<SingleAreaLight> areaLight
        = std::make_shared<SingleAreaLight>(primitive->second.get(), Spectrum(color), watt, isBackFaceEmit);

    primitive->second->setAreaLight(areaLight.get());

    return areaLight;
}

static std::shared_ptr<Light> createEnvironment(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives,
    std::shared_ptr<Primitive>&    out_backgroundSphere) {
    
    const auto hdrFilename = data->findString("hdr-filename");

    CADISE_ASSERT_NE(hdrFilename, "");

    HdrImage hdrImage = PictureLoader::loadRgbImage(Path(hdrFilename));
    hdrImage.flipHorizontal();

    const ETextureSampleMode sampleMode = ETextureSampleMode::NEAREST;
    const ETextureWrapMode   wrapMode   = ETextureWrapMode::REPEAT;
    const std::shared_ptr<TTexture<Spectrum>> radiance
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
    std::shared_ptr<Primitive>&    out_backgroundSphere) {

    CADISE_ASSERT(data);

    std::shared_ptr<Light> light = nullptr;
    const auto type = data->findString("type");
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