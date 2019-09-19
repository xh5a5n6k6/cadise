#include "core/renderOption.h"

#include "core/bsdf/bsdf.h"
#include "core/instantiator/instantiator.h"
#include "core/intersector/primitive/primitive.h"
#include "core/light/light.h"
#include "core/renderer/renderer.h"
#include "core/scene.h"
#include "core/texture/texture.h"

#include "file-io/scene-description/sdData.h"

namespace cadise {

RenderOption::RenderOption() :
    _cameraData(nullptr),
    _rendererData(nullptr),
    _acceleratorData(nullptr),
    _scene(nullptr),
    _renderer(nullptr) {
}

void RenderOption::setUpData(const std::shared_ptr<SdData>& data) {
    switch (data->classType()) {
        case SdClassType::CAMERA:
            _setUpCamera(data);
            break;
        case SdClassType::RENDERER:
            _setUpRenderer(data);
            break;
        case SdClassType::ACCELERATOR:
            _setUpAccelerator(data);
            break;
        case SdClassType::TEXTURE_REAL:
            _setUpRealTexture(data);
            break;
        case SdClassType::TEXTURE_SPECTRUM:
            _setUpSpectrumTexture(data);
            break;
        case SdClassType::MATERIAL:
            _setUpBsdf(data);
            break;
        case SdClassType::LIGHT:
            _setUpLight(data);
            break;
        case SdClassType::PRIMITIVE:
            _setUpPrimitive(data);
            break;
        default:
            break;
    }
}

void RenderOption::prepareRender() {
    _bsdfs.clear();
    _primitives.clear();
    _realTextures.clear();
    _spectrumTextures.clear();

    std::shared_ptr<Accelerator> accelerator = instantiator::makeAccelerator(_acceleratorData, _intersectors);
    std::shared_ptr<Camera> camera = instantiator::makeCamera(_cameraData);

    _scene = std::make_shared<Scene>(std::move(accelerator), std::move(_lights));
    _renderer = std::move(instantiator::makeRenderer(_rendererData));
    
    _renderer->setCamera(std::move(camera));
}

void RenderOption::startRender() const {
    _renderer->render(*_scene);
}

void RenderOption::_setUpCamera(const std::shared_ptr<SdData>& data) {
    _cameraData = std::move(data);
}

void RenderOption::_setUpRenderer(const std::shared_ptr<SdData>& data) {
    _rendererData = std::move(data);
}

void RenderOption::_setUpAccelerator(const std::shared_ptr<SdData>& data) {
    _acceleratorData = std::move(data);
}

void RenderOption::_setUpRealTexture(const std::shared_ptr<SdData>& data) {
    std::shared_ptr<Texture<real>> texture
        = instantiator::makeRealTexture(data, _realTextures, _spectrumTextures);
    std::string_view textureName = data->findString("name");

    _realTextures.insert(
        std::pair<std::string, std::shared_ptr<Texture<real>>>(textureName, texture));
}

void RenderOption::_setUpSpectrumTexture(const std::shared_ptr<SdData>& data) {
    std::shared_ptr<Texture<Spectrum>> texture
        = instantiator::makeSpectrumTexture(data, _realTextures, _spectrumTextures);
    std::string_view textureName = data->findString("name");

    _spectrumTextures.insert(
        std::pair<std::string, std::shared_ptr<Texture<Spectrum>>>(textureName, texture));
}

void RenderOption::_setUpBsdf(const std::shared_ptr<SdData>& data) {
    std::shared_ptr<Bsdf> bsdf = instantiator::makeBsdf(data, _realTextures, _spectrumTextures);
    std::string_view bsdfName = data->findString("name");

    _bsdfs.insert(std::pair<std::string, std::shared_ptr<Bsdf>>(bsdfName, bsdf));
}

void RenderOption::_setUpLight(const std::shared_ptr<SdData>& data) {
    std::shared_ptr<Light> light = instantiator::makeLight(data, _primitives);
    
    _lights.push_back(light);
}

void RenderOption::_setUpPrimitive(const std::shared_ptr<SdData>& data) {
    std::shared_ptr<Primitive> primitive = instantiator::makePrimitive(data, _bsdfs);
    std::string_view primitiveName = data->findString("name");

    if (primitiveName != "") {
        _primitives.insert(std::pair<std::string, std::shared_ptr<Primitive>>(primitiveName, primitive));
        _intersectors.push_back(primitive);
    }
    else {
        _intersectors.push_back(primitive);
    }
}

} // namespace cadise