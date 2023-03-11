#include "Core/RenderDatabase.h"

#include "Core/Camera/Camera.h"
#include "Core/Film/Film.h"
#include "Core/Instantiator/Instantiator.h"
#include "Core/Intersector/Accelerator/Accelerator.h"
#include "Core/Intersector/Primitive/Primitive.h"
#include "Core/Intersector/Primitive/TriangleBuffer.h"
#include "Core/Light/Light.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Scene.h"
#include "Core/Surface/BSDF/BSDF.h"
#include "Core/Texture/TTexture.h"
#include "FileIO/CSD/CSDResource.h"
#include "Foundation/Assertion.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Time/Stopwatch.h"
#include "Math/TAABB3.h"

#include <limits>

namespace cadise 
{

namespace 
{
    const Logger logger("Database");
}

RenderDatabase::RenderDatabase() :
    _filmResource(nullptr),
    _cameraResource(nullptr),
    _rendererResource(nullptr),
    _acceleratorResource(nullptr),
    _backgroundSphere(nullptr),
    _environmentLightIndex(std::numeric_limits<std::size_t>::max()) 
{}

void RenderDatabase::consumeResource(const std::shared_ptr<CSDResource>& resource) 
{
    CS_ASSERT(resource);

    switch (resource->classType())
    {
        case ECSDClassType::Film:
            _setUpFilm(resource);
            break;

        case ECSDClassType::Camera:
            _setUpCamera(resource);
            break;

        case ECSDClassType::Renderer:
            _setUpRenderer(resource);
            break;

        case ECSDClassType::Accelerator:
            _setUpAccelerator(resource);
            break;

        case ECSDClassType::LightCluster:
            _setUpLightCluster(resource);
            break;

        case ECSDClassType::TextureReal:
            _setUpRealTexture(resource);
            break;

        case ECSDClassType::TextureSpectrum:
            _setUpSpectrumTexture(resource);
            break;

        case ECSDClassType::Material:
            _setUpBsdf(resource);
            break;

        case ECSDClassType::Light:
            _setUpLight(resource);
            break;

        case ECSDClassType::Primitive:
            _setUpPrimitive(resource);
            break;

        default:
            break;
    }
}

std::shared_ptr<Renderer> RenderDatabase::prepareRender() 
{
    // make all triangle meshes to triangles
    if (!_triangleBuffers.empty())
    {
        for (const auto& pair : _triangleBuffers)
        {
            std::vector<std::shared_ptr<Primitive>> triangles;
            pair.second->transformToTriangles(&triangles);

            for (std::size_t i = 0; i < triangles.size(); ++i)
            {
                _intersectors.push_back(triangles[i]);
            }
        }
    }

    _bsdfs.clear();
    _primitives.clear();
    _triangleBuffers.clear();
    _realTextures.clear();
    _spectrumTextures.clear();

    logger.log("Finished loading scene objects (" + std::to_string(_intersectors.size()) + " primitives)");

    const std::shared_ptr<Film>   film   = instantiator::makeFilm(_filmResource);
    const std::shared_ptr<Camera> camera = instantiator::makeCamera(_cameraResource);

    logger.log("Building primitive accelerator");

    Stopwatch stopwatch;
    stopwatch.start();

    const std::shared_ptr<Accelerator> accelerator = instantiator::makeAccelerator(_acceleratorResource, _intersectors);

    stopwatch.stop();

    logger.log("Finished building accelerator (" + stopwatch.elapsedTime().toString() + ")");

    // HACK
    // it means there is an environment light
    if (_environmentLightIndex != std::numeric_limits<std::size_t>::max()) 
    {
        AABB3R sceneBound;
        accelerator->evaluateBound(&sceneBound);

        if (!sceneBound.isEmpty()) 
        {
            const real sceneBoundRadius = sceneBound.extent().length() * 0.5_r;

            _lights[_environmentLightIndex]->setSceneBoundRadius(sceneBoundRadius);
        }
    }

    const std::shared_ptr<LightCluster> lightCluster = instantiator::makeLightCluster(_lightClusterResource, _lights);
    const std::shared_ptr<Scene>        scene        = std::make_shared<Scene>(accelerator, lightCluster);
    if (_backgroundSphere)
    {
        scene->setBackgroundSphere(_backgroundSphere.get());
    }

    camera->setResolution(film->resolution().asType<std::size_t>());
    camera->updateTransform();

    std::shared_ptr<Renderer> renderer = instantiator::makeRenderer(_rendererResource);
    renderer->setScene(scene);
    renderer->setCamera(camera);
    renderer->setFilm(film);

    return renderer;
}

void RenderDatabase::_setUpFilm(const std::shared_ptr<CSDResource>& resource) 
{
    _filmResource = resource;
}

void RenderDatabase::_setUpCamera(const std::shared_ptr<CSDResource>& resource)
{
    _cameraResource = resource;
}

void RenderDatabase::_setUpRenderer(const std::shared_ptr<CSDResource>& resource)
{
    _rendererResource = resource;
}

void RenderDatabase::_setUpAccelerator(const std::shared_ptr<CSDResource>& resource)
{
    _acceleratorResource = resource;
}

void RenderDatabase::_setUpLightCluster(const std::shared_ptr<CSDResource>& resource)
{
    _lightClusterResource = resource;
}

void RenderDatabase::_setUpRealTexture(const std::shared_ptr<CSDResource>& resource)
{
    const std::shared_ptr<TTexture<real>> texture
        = instantiator::makeRealTexture(resource, _realTextures, _spectrumTextures);
    const std::string textureName = resource->findString("name");

    _realTextures.insert(
        std::pair<std::string, std::shared_ptr<TTexture<real>>>(textureName, texture));
}

void RenderDatabase::_setUpSpectrumTexture(const std::shared_ptr<CSDResource>& resource)
{
    const std::shared_ptr<TTexture<Spectrum>> texture
        = instantiator::makeSpectrumTexture(resource, _realTextures, _spectrumTextures);
    const std::string textureName = resource->findString("name");

    _spectrumTextures.insert(
        std::pair<std::string, std::shared_ptr<TTexture<Spectrum>>>(textureName, texture));
}

void RenderDatabase::_setUpBsdf(const std::shared_ptr<CSDResource>& resource)
{
    const std::shared_ptr<BSDF> bsdf = instantiator::makeBsdf(resource, _realTextures, _spectrumTextures, _bsdfs);
    const std::string bsdfName = resource->findString("name");

    _bsdfs.insert(std::pair<std::string, std::shared_ptr<BSDF>>(bsdfName, bsdf));
}

void RenderDatabase::_setUpLight(const std::shared_ptr<CSDResource>& resource)
{
    instantiator::makeLight(
        resource,
        _primitives,
        _triangleBuffers,
        _lights,
        _intersectors,
        _backgroundSphere);
    
    // HACK
    if (_backgroundSphere) 
    {
        _environmentLightIndex = _lights.size() - 1;
    }
}

void RenderDatabase::_setUpPrimitive(const std::shared_ptr<CSDResource>& resource)
{
    instantiator::makePrimitive(resource, _bsdfs, _intersectors, _primitives, _triangleBuffers);
}

} // namespace cadise