#pragma once

#include "Core/Spectrum/Spectrum.h"
#include "Math/Type/MapType.h"

#include <vector>

// forward declaration
namespace cadise
{
    class Accelerator;
    class BSDF;
    class Camera;
    class CSDResource;
    class EnergyEstimator;
    class Film;
    class Filter;
    class Intersector;
    class Light;
    class LightCluster;
    class Primitive;
    class Renderer;
    class Sampler;
    class Scene;
    class TriangleBuffer;

    template<typename T>
    class TTexture;
}

// TODO: maybe use factory to deal with object instantiation
namespace cadise::instantiator
{

// implement in Core/instantiator/cameraInstantiator.cpp
std::shared_ptr<Camera> makeCamera(
    const std::shared_ptr<CSDResource>& data);

// implement in Core/instantiator/rendererInstantiator.cpp
std::shared_ptr<Renderer> makeRenderer(
    const std::shared_ptr<CSDResource>& data);

// implement in Core/instantiator/estimatorInstantiator.cpp
std::shared_ptr<EnergyEstimator> makeEstimator(
    const std::shared_ptr<CSDResource>& data);

// implement in Core/instantiator/filmInstantiator.cpp
std::shared_ptr<Film> makeFilm(
    const std::shared_ptr<CSDResource>& data);

// implement in Core/instantiator/filterInstantiator.cpp
std::shared_ptr<Filter> makeFilter(
    const std::shared_ptr<CSDResource>& data);

// implement in Core/instantiator/samplerInstantiator.cpp
std::shared_ptr<Sampler> makeSampler(
    const std::shared_ptr<CSDResource>& data);

// implement in Core/instantiator/acceleratorInstantiator.cpp
std::shared_ptr<Accelerator> makeAccelerator(
    const std::shared_ptr<CSDResource>&              data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors);

// implement in Core/instantiator/lightClusterInstantiator.cpp
std::shared_ptr<LightCluster> makeLightCluster(
    const std::shared_ptr<CSDResource>&        data,
    const std::vector<std::shared_ptr<Light>>& lights);

// implement in Core/instantiator/textureInstantiator.cpp
std::shared_ptr<TTexture<real>> makeRealTexture(
    const std::shared_ptr<CSDResource>&      data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures);
std::shared_ptr<TTexture<Spectrum>> makeSpectrumTexture(
    const std::shared_ptr<CSDResource>&      data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures);

// implement in Core/instantiator/bsdfInstantiator.cpp
std::shared_ptr<BSDF> makeBsdf(
    const std::shared_ptr<CSDResource>&      data,
    const TStringKeyMap<TTexture<real>>&     realTextures,
    const TStringKeyMap<TTexture<Spectrum>>& spectrumTextures,
    const TStringKeyMap<BSDF>&               bsdfs);

// implement in Core/instantiator/lightInstantiator.cpp
void makeLight(
    const std::shared_ptr<CSDResource>&         data,
    const TStringKeyMap<Primitive>&             primitives,
    TStringKeyMap<TriangleBuffer>&              out_triangleBuffers,
    std::vector<std::shared_ptr<Light>>&        out_lights,
    std::vector<std::shared_ptr<Intersector>>&  out_intersectors,
    std::shared_ptr<Primitive>&                 out_backgroundSphere);

// implement in Core/instantiator/primitiveInstantiator.cpp
void makePrimitive(
    const std::shared_ptr<CSDResource>&         data,
    const TStringKeyMap<BSDF>&                  bsdfs,
    std::vector<std::shared_ptr<Intersector>>&  out_intersectors,
    TStringKeyMap<Primitive>&                   out_primitives,
    TStringKeyMap<TriangleBuffer>&              out_triangleBuffers);

} // namespace cadise::instantiator