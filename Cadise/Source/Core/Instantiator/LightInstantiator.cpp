#include "core/instantiator/instantiator.h"

// light type
#include "core/light/category/environmentLight.h"
#include "core/light/category/pointLight.h"
#include "core/light/category/singleAreaLight.h"

#include "core/imaging/tImage.h"
#include "core/intersector/primitive/infiniteSphere.h"
#include "core/intersector/primitive/primitive.h"
#include "core/intersector/primitive/triangleBuffer.h"
#include "core/texture/category/rgbImageTexture.h"
#include "core/texture/category/tConstantTexture.h"
#include "file-io/path.h"
#include "file-io/pictureLoader.h"
#include "file-io/scene-description/CSDResource.h"
#include "fundamental/assertion.h"

namespace cadise::instantiator
{

static std::shared_ptr<Light> createPoint(
    const std::shared_ptr<CSDResource>&  data,
    const TStringKeyMap<Primitive>& primitives)
{
    const auto position  = data->findVector3<real>("position");
    const auto intensity = data->findVector3<real>("intensity");

    return std::make_shared<PointLight>(position, Spectrum(intensity));
}

static std::vector<std::shared_ptr<AreaLight>> createArea(
    const std::shared_ptr<CSDResource>&             data,
    const TStringKeyMap<Primitive>&            primitives,
    TStringKeyMap<TriangleBuffer>&             out_triangleBuffers,
    std::vector<std::shared_ptr<Intersector>>& out_intersectors)
{
    std::vector<std::shared_ptr<AreaLight>> lights;

    const auto color          = data->findVector3<real>("color");
    const real watt           = data->findFloat<real>("watt");
    const bool isBackFaceEmit = data->findBool("is-back-face-emit");

    const auto   primitiveName      = data->findString("primitive");
    const auto&& primitivePair      = primitives.find(primitiveName);
    const auto&& triangleBufferPair = out_triangleBuffers.find(primitiveName);

    CS_ASSERT(
        primitivePair != primitives.end() ||
        triangleBufferPair != out_triangleBuffers.end());

    if (primitivePair != primitives.end())
    {
        const auto areaLight = std::make_shared<SingleAreaLight>(
            primitivePair->second.get(),
            Spectrum(color), 
            watt, 
            isBackFaceEmit);

        primitivePair->second->setAreaLight(areaLight.get());

        lights.push_back(areaLight);
    }
    else
    {
        std::vector<std::shared_ptr<Primitive>> triangles;
        triangleBufferPair->second->transformToTriangles(&triangles);

        real totalAreas = 0;
        for (std::size_t i = 0; i < triangles.size(); ++i)
        {
            totalAreas += triangles[i]->area();

            const auto areaLight = std::make_shared<SingleAreaLight>(
                triangles[i].get(),
                Spectrum(color),
                watt,
                isBackFaceEmit);

            triangles[i]->setAreaLight(areaLight.get());

            lights.push_back(areaLight);
            out_intersectors.push_back(triangles[i]);
        }

        const Spectrum colorSpectrum  = Spectrum(color);
        const Spectrum unitWattColor  = colorSpectrum.div(colorSpectrum.sum());
        const Spectrum totalWattColor = unitWattColor.mul(watt);

        auto emitRadiance = std::make_shared<TConstantTexture<Spectrum>>(
            totalWattColor.div(totalAreas * constant::pi<real>));

        for (std::size_t i = 0; i < lights.size(); ++i)
        {
            lights[i]->setEmitRadiance(emitRadiance);
        }

        out_triangleBuffers.erase(std::string(primitiveName));
    }

    return lights;
}

static std::shared_ptr<Light> createEnvironment(
    const std::shared_ptr<CSDResource>&  data,
    const TStringKeyMap<Primitive>& primitives,
    std::shared_ptr<Primitive>&     out_backgroundSphere) 
{
    const auto hdrFilename = data->findString("hdr-filename");

    CS_ASSERT_NE(hdrFilename, "");

    HdrImage hdrImage = PictureLoader::loadRgbImage(Path(hdrFilename));
    hdrImage.flipHorizontal();

    const ETextureSampleMode sampleMode = ETextureSampleMode::Nearest;
    const ETextureWrapMode   wrapMode   = ETextureWrapMode::Repeat;
    const std::shared_ptr<TTexture<Spectrum>> radiance
        = std::make_shared<RgbImageTexture>(hdrImage, sampleMode, wrapMode);

    out_backgroundSphere = std::make_shared<InfiniteSphere>();
    std::shared_ptr<EnvironmentLight> environmentLight
        = std::make_shared<EnvironmentLight>(out_backgroundSphere.get(), 
                                             radiance, 
                                             hdrImage.resolution().asType<std::size_t>());

    out_backgroundSphere->setAreaLight(environmentLight.get());

    return environmentLight;
}

void makeLight(
    const std::shared_ptr<CSDResource>&             data,
    const TStringKeyMap<Primitive>&            primitives,
    TStringKeyMap<TriangleBuffer>&             out_triangleBuffers,
    std::vector<std::shared_ptr<Light>>&       out_lights,
    std::vector<std::shared_ptr<Intersector>>& out_intersectors,
    std::shared_ptr<Primitive>&                out_backgroundSphere)
{
    CS_ASSERT(data);

    std::shared_ptr<Light> light = nullptr;
    const auto type = data->findString("type");
    if (type == "point") 
    {
        light = createPoint(data, primitives);
        out_lights.push_back(light);
    }
    else if (type == "area")
    {
        const auto lights = createArea(data, primitives, out_triangleBuffers, out_intersectors);
        for (std::size_t i = 0; i < lights.size(); ++i)
        {
            out_lights.push_back(lights[i]);
        }
    }
    else if (type == "environment") 
    {
        light = createEnvironment(data, primitives, out_backgroundSphere);
        out_lights.push_back(light);
    }
    else 
    {
        // unsupported light type
    }
}

} // namespace cadise::instantiator