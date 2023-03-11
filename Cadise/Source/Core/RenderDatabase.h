#pragma once

#include "Core/Spectrum/Spectrum.h"
#include "Math/Type/MapType.h"

#include <memory>
#include <vector>

// forward declaration
namespace cadise 
{ 
    class BSDF;
    class CSDResource;
    class Intersector;
    class Light;
    class Primitive;
    class Renderer;
    class Scene;
    class TriangleBuffer;

    template<typename T>
    class TTexture;
}

namespace cadise 
{

class RenderDatabase
{
public:
    RenderDatabase();

    void consumeResource(const std::shared_ptr<CSDResource>& resource);

    std::shared_ptr<Renderer> prepareRender();

private:
    void _setUpFilm(const std::shared_ptr<CSDResource>& resource);
    void _setUpCamera(const std::shared_ptr<CSDResource>& resource);
    void _setUpRenderer(const std::shared_ptr<CSDResource>& resource);
    void _setUpAccelerator(const std::shared_ptr<CSDResource>& resource);
    void _setUpLightCluster(const std::shared_ptr<CSDResource>& resource);
    void _setUpRealTexture(const std::shared_ptr<CSDResource>& resource);
    void _setUpSpectrumTexture(const std::shared_ptr<CSDResource>& resource);
    void _setUpBsdf(const std::shared_ptr<CSDResource>& resource);
    void _setUpLight(const std::shared_ptr<CSDResource>& resource);
    void _setUpPrimitive(const std::shared_ptr<CSDResource>& resource);

    std::shared_ptr<CSDResource> _filmResource;
    std::shared_ptr<CSDResource> _cameraResource;
    std::shared_ptr<CSDResource> _rendererResource;
    std::shared_ptr<CSDResource> _acceleratorResource;
    std::shared_ptr<CSDResource> _lightClusterResource;

    std::vector<std::shared_ptr<Intersector>> _intersectors;
    std::vector<std::shared_ptr<Light>>       _lights;

    // HACK
    std::shared_ptr<Primitive> _backgroundSphere;
    std::size_t                _environmentLightIndex;

    TStringKeyMap<BSDF>               _bsdfs;
    TStringKeyMap<Primitive>          _primitives; // for area light
    TStringKeyMap<TriangleBuffer>     _triangleBuffers; // HACK for non-decomposed triangles
    TStringKeyMap<TTexture<real>>     _realTextures;
    TStringKeyMap<TTexture<Spectrum>> _spectrumTextures;
};

} // namespace cadise