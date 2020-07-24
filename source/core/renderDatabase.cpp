#include "core/renderDatabase.h"

#include "core/camera/camera.h"
#include "core/film/film.h"
#include "core/instantiator/instantiator.h"
#include "core/intersector/accelerator/accelerator.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/renderer/renderer.h"
#include "core/scene.h"
#include "core/surface/bsdf/bsdf.h"
#include "core/texture/texture.h"
#include "file-io/scene-description/sdData.h"
#include "fundamental/assertion.h"
#include "fundamental/logger/logger.h"
#include "fundamental/time/stopwatch.h"
#include "math/aabb.h"

#include <limits>

namespace cadise {

// local logger declaration
namespace {
    const Logger logger("Database");
} // anonymous namespace

RenderDatabase::RenderDatabase() :
    _filmData(nullptr),
    _cameraData(nullptr),
    _rendererData(nullptr),
    _acceleratorData(nullptr),
    _scene(nullptr),
    _renderer(nullptr),
    _backgroundSphere(nullptr),
    _environmentLightIndex(std::numeric_limits<std::size_t>::max()) {
}

void RenderDatabase::setUpData(const std::shared_ptr<SdData>& data) {
    CADISE_ASSERT(data);

    switch (data->classType()) {
        case ESdClassType::FILM:
            _setUpFilm(data);
            break;
        case ESdClassType::CAMERA:
            _setUpCamera(data);
            break;
        case ESdClassType::RENDERER:
            _setUpRenderer(data);
            break;
        case ESdClassType::ACCELERATOR:
            _setUpAccelerator(data);
            break;
        case ESdClassType::LIGHT_CLUSTER:
            _setUpLightCluster(data);
            break;
        case ESdClassType::TEXTURE_REAL:
            _setUpRealTexture(data);
            break;
        case ESdClassType::TEXTURE_SPECTRUM:
            _setUpSpectrumTexture(data);
            break;
        case ESdClassType::MATERIAL:
            _setUpBsdf(data);
            break;
        case ESdClassType::LIGHT:
            _setUpLight(data);
            break;
        case ESdClassType::PRIMITIVE:
            _setUpPrimitive(data);
            break;
        default: 
            break;    
    }
}

void RenderDatabase::prepareRender() {
    _bsdfs.clear();
    _primitives.clear();
    _realTextures.clear();
    _spectrumTextures.clear();

    logger.log("Finished loading scene objects (" + std::to_string(_intersectors.size()) + " primitives)");

    const std::shared_ptr<Film> film = instantiator::makeFilm(_filmData);
    const std::shared_ptr<Camera> camera = instantiator::makeCamera(_cameraData);

    logger.log("Building primitive accelerator");

    Stopwatch stopwatch;
    stopwatch.start();

    const std::shared_ptr<Accelerator> accelerator = instantiator::makeAccelerator(_acceleratorData, _intersectors);

    stopwatch.stop();

    logger.log("Finished building accelerator (" + stopwatch.elapsedTime().format() + ")");

    // HACK
    // it means there is an environment light
    if (_environmentLightIndex != std::numeric_limits<std::size_t>::max()) {
        AABB3R sceneBound;
        accelerator->evaluateBound(&sceneBound);

        if (!sceneBound.isEmpty()) {
            const real sceneBoundRadius = sceneBound.extent().length() * 0.5_r;

            _lights[_environmentLightIndex]->setSceneBoundRadius(sceneBoundRadius);
        }
    }

    const std::shared_ptr<LightCluster> lightCluster = instantiator::makeLightCluster(_lightClusterData, _lights);

    _scene = std::make_shared<Scene>(accelerator, lightCluster);
    _renderer = std::move(instantiator::makeRenderer(_rendererData));
    
    if (_backgroundSphere) {
        _scene->setBackgroundSphere(_backgroundSphere.get());
    }

    const real rx = static_cast<real>(film->resolution().x());
    const real ry = static_cast<real>(film->resolution().y());
    camera->setAspectRatio(rx / ry);
    camera->updateTransform();

    _renderer->setScene(_scene.get());
    _renderer->setCamera(camera);
    _renderer->setFilm(film);
}

void RenderDatabase::startRender() const {
    _renderer->render();
}

void RenderDatabase::_setUpFilm(const std::shared_ptr<SdData>& data) {
    _filmData = std::move(data);
}

void RenderDatabase::_setUpCamera(const std::shared_ptr<SdData>& data) {
    _cameraData = std::move(data);
}

void RenderDatabase::_setUpRenderer(const std::shared_ptr<SdData>& data) {
    _rendererData = std::move(data);
}

void RenderDatabase::_setUpAccelerator(const std::shared_ptr<SdData>& data) {
    _acceleratorData = std::move(data);
}

void RenderDatabase::_setUpLightCluster(const std::shared_ptr<SdData>& data) {
    _lightClusterData = std::move(data);
}

void RenderDatabase::_setUpRealTexture(const std::shared_ptr<SdData>& data) {
    const std::shared_ptr<Texture<real>> texture
        = instantiator::makeRealTexture(data, _realTextures, _spectrumTextures);
    const std::string_view textureName = data->findString("name");

    _realTextures.insert(
        std::pair<std::string, std::shared_ptr<Texture<real>>>(textureName, texture));
}

void RenderDatabase::_setUpSpectrumTexture(const std::shared_ptr<SdData>& data) {
    const std::shared_ptr<Texture<Spectrum>> texture
        = instantiator::makeSpectrumTexture(data, _realTextures, _spectrumTextures);
    const std::string_view textureName = data->findString("name");

    _spectrumTextures.insert(
        std::pair<std::string, std::shared_ptr<Texture<Spectrum>>>(textureName, texture));
}

void RenderDatabase::_setUpBsdf(const std::shared_ptr<SdData>& data) {
    const std::shared_ptr<Bsdf> bsdf = instantiator::makeBsdf(data, _realTextures, _spectrumTextures, _bsdfs);
    const std::string_view bsdfName = data->findString("name");

    _bsdfs.insert(std::pair<std::string, std::shared_ptr<Bsdf>>(bsdfName, bsdf));
}

void RenderDatabase::_setUpLight(const std::shared_ptr<SdData>& data) {
    const std::shared_ptr<Light> light = instantiator::makeLight(data, _primitives, _backgroundSphere);
    
    // HACK
    if (_backgroundSphere) {
        _environmentLightIndex = _lights.size();
    }

    _lights.push_back(light);
}

void RenderDatabase::_setUpPrimitive(const std::shared_ptr<SdData>& data) {
    instantiator::makePrimitive(data, _bsdfs, &_intersectors, &_primitives);
}

} // namespace cadise