#pragma once

#include "core/spectrum/spectrum.h"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cadise {

class BSDF;
class Intersector;
class Light;
class Primitive;
class Renderer;
class Scene;
class SdData;
template<typename T>
class Texture;

class RenderOption {
public:
    RenderOption();

    void setUpData(const std::shared_ptr<SdData>& data);

    std::unique_ptr<Scene> makeScene() const;
    std::unique_ptr<Renderer> makeRenderer() const;

private:
    void _setUpCamera(const std::shared_ptr<SdData>& data);
    void _setUpRenderer(const std::shared_ptr<SdData>& data);
    void _setUpAccelerator(const std::shared_ptr<SdData>& data);
    void _setUpRealTexture(const std::shared_ptr<SdData>& data);
    void _setUpSpectrumTexture(const std::shared_ptr<SdData>& data);
    void _setUpBsdf(const std::shared_ptr<SdData>& data);
    void _setUpLight(const std::shared_ptr<SdData>& data);
    void _setUpPrimitive(const std::shared_ptr<SdData>& data);

    std::shared_ptr<SdData> _cameraData;
    std::shared_ptr<SdData> _rendererData;
    std::shared_ptr<SdData> _acceleratorData;

    std::vector<std::shared_ptr<Intersector>> _intersectors;
    std::vector<std::shared_ptr<Light>>       _lights;

    std::map<std::string, std::shared_ptr<BSDF>, std::less<>>              _bsdfs;
    std::map<std::string, std::shared_ptr<Primitive>, std::less<>>         _primitives; // for area lights
    std::map<std::string, std::shared_ptr<Texture<real>>, std::less<>>     _realTextures;
    std::map<std::string, std::shared_ptr<Texture<Spectrum>>, std::less<>> _spectrumTextures;
};

} // namespace cadise