#pragma once

#include "core/spectrum/spectrum.h"
#include "math/type/mapType.h"

#include <memory>
#include <vector>

// forward declaration
namespace cadise 
{ 
    template<typename T>
    class TTexture;

    class Bsdf;
    class Intersector;
    class Light;
    class Primitive;
    class Renderer;
    class Scene;
    class SdData;
    class TriangleBuffer;
} // end forward declaration

namespace cadise 
{

class RenderDatabase
{
public:
    RenderDatabase();

    void setUpData(const std::shared_ptr<SdData>& data);

    void prepareRender();
    void render() const;

private:
    void _setUpFilm(const std::shared_ptr<SdData>& data);
    void _setUpCamera(const std::shared_ptr<SdData>& data);
    void _setUpRenderer(const std::shared_ptr<SdData>& data);
    void _setUpAccelerator(const std::shared_ptr<SdData>& data);
    void _setUpLightCluster(const std::shared_ptr<SdData>& data);
    void _setUpRealTexture(const std::shared_ptr<SdData>& data);
    void _setUpSpectrumTexture(const std::shared_ptr<SdData>& data);
    void _setUpBsdf(const std::shared_ptr<SdData>& data);
    void _setUpLight(const std::shared_ptr<SdData>& data);
    void _setUpPrimitive(const std::shared_ptr<SdData>& data);

    std::shared_ptr<SdData> _filmData;
    std::shared_ptr<SdData> _cameraData;
    std::shared_ptr<SdData> _rendererData;
    std::shared_ptr<SdData> _acceleratorData;
    std::shared_ptr<SdData> _lightClusterData;

    std::shared_ptr<Scene>    _scene;
    std::shared_ptr<Renderer> _renderer;

    std::vector<std::shared_ptr<Intersector>> _intersectors;
    std::vector<std::shared_ptr<Light>>       _lights;

    // HACK
    std::shared_ptr<Primitive> _backgroundSphere;
    std::size_t                _environmentLightIndex;

    StringKeyMap<Bsdf>               _bsdfs;
    StringKeyMap<Primitive>          _primitives; // for area light
    StringKeyMap<TriangleBuffer>     _triangleBuffers; // HACK for non-decomposed triangles
    StringKeyMap<TTexture<real>>     _realTextures;
    StringKeyMap<TTexture<Spectrum>> _spectrumTextures;
};

} // namespace cadise