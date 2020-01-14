#pragma once

#include "core/spectrum/spectrum.h"
#include "math/type/mapType.h"

namespace cadise {

class Accelerator;
class Bsdf;
class Camera;
class Film;
class Filter;
class Integrator;
class Intersector;
class Light;
class Primitive;
class Renderer;
class Sampler;
class Scene;
class SdData;
template<typename T>
class Texture;

namespace instantiator {

// implement in core/instantiator/cameraInstantiator.cpp
std::shared_ptr<Camera> makeCamera(const std::shared_ptr<SdData>& data);

// implement in core/instantiator/rendererInstantiator.cpp
std::shared_ptr<Renderer> makeRenderer(const std::shared_ptr<SdData>& data);

// implement in core/instantiator/integratorInstantiator.cpp
std::shared_ptr<Integrator> makeIntegrator(const std::shared_ptr<SdData>& data);

// implement in core/instantiator/filmInstantiator.cpp
std::shared_ptr<Film> makeFilm(const std::shared_ptr<SdData>& data);

// implement in core/instantiator/filterInstantiator.cpp
std::shared_ptr<Filter> makeFilter(const std::shared_ptr<SdData>& data);

// implement in core/instantiator/samplerInstantiator.cpp
std::shared_ptr<Sampler> makeSampler(const std::shared_ptr<SdData>& data);

// implement in core/instantiator/acceleratorInstantiator.cpp
std::shared_ptr<Accelerator> makeAccelerator(
    const std::shared_ptr<SdData>& data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors);

// implement in core/instantiator/textureInstantiator.cpp
std::shared_ptr<Texture<real>> makeRealTexture(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures);
std::shared_ptr<Texture<Spectrum>> makeSpectrumTexture(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures);

// implement in core/instantiator/bsdfInstantiator.cpp
std::shared_ptr<Bsdf> makeBsdf(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Texture<real>>& realTextures,
    const StringKeyMap<Texture<Spectrum>>& spectrumTextures,
    const StringKeyMap<Bsdf>& bsdfs);

// implement in core/instantiator/lightInstantiator.cpp
std::shared_ptr<Light> makeLight(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives,
    std::shared_ptr<Primitive>& out_infiniteSphere);

// implement in core/instantiator/primitiveInstantiator.cpp
void makePrimitive(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Bsdf>& bsdfs,
    std::vector<std::shared_ptr<Intersector>>* const out_intersectors,
    StringKeyMap<Primitive>* const out_primitives);

} // namespace instantiator

} // namespace cadise