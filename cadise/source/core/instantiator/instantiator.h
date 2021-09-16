#pragma once

#include "core/spectrum/spectrum.h"
#include "math/type/mapType.h"

namespace cadise { class Accelerator; }
namespace cadise { class Bsdf; }
namespace cadise { class Camera; }
namespace cadise { class EnergyEstimator; }
namespace cadise { class Film; }
namespace cadise { class Filter; }
namespace cadise { class Intersector; }
namespace cadise { class Light; }
namespace cadise { class LightCluster; }
namespace cadise { class Primitive; }
namespace cadise { class Renderer; }
namespace cadise { class Sampler; }
namespace cadise { class Scene; }
namespace cadise { class SdData; }
namespace cadise { template<typename T> class TTexture; }

namespace cadise::instantiator
{

// implement in core/instantiator/cameraInstantiator.cpp
std::shared_ptr<Camera> makeCamera(
    const std::shared_ptr<SdData>& data);

// implement in core/instantiator/rendererInstantiator.cpp
std::shared_ptr<Renderer> makeRenderer(
    const std::shared_ptr<SdData>& data);

// implement in core/instantiator/estimatorInstantiator.cpp
std::shared_ptr<EnergyEstimator> makeEstimator(
    const std::shared_ptr<SdData>& data);

// implement in core/instantiator/filmInstantiator.cpp
std::shared_ptr<Film> makeFilm(
    const std::shared_ptr<SdData>& data);

// implement in core/instantiator/filterInstantiator.cpp
std::shared_ptr<Filter> makeFilter(
    const std::shared_ptr<SdData>& data);

// implement in core/instantiator/samplerInstantiator.cpp
std::shared_ptr<Sampler> makeSampler(
    const std::shared_ptr<SdData>& data);

// implement in core/instantiator/acceleratorInstantiator.cpp
std::shared_ptr<Accelerator> makeAccelerator(
    const std::shared_ptr<SdData>&                   data,
    const std::vector<std::shared_ptr<Intersector>>& intersectors);

// implement in core/instantiator/lightClusterInstantiator.cpp
std::shared_ptr<LightCluster> makeLightCluster(
    const std::shared_ptr<SdData>&             data,
    const std::vector<std::shared_ptr<Light>>& lights);

// implement in core/instantiator/textureInstantiator.cpp
std::shared_ptr<TTexture<real>> makeRealTexture(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures);
std::shared_ptr<TTexture<Spectrum>> makeSpectrumTexture(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures);

// implement in core/instantiator/bsdfInstantiator.cpp
std::shared_ptr<Bsdf> makeBsdf(
    const std::shared_ptr<SdData>&          data,
    const StringKeyMap<TTexture<real>>&     realTextures,
    const StringKeyMap<TTexture<Spectrum>>& spectrumTextures,
    const StringKeyMap<Bsdf>&               bsdfs);

// implement in core/instantiator/lightInstantiator.cpp
std::shared_ptr<Light> makeLight(
    const std::shared_ptr<SdData>& data,
    const StringKeyMap<Primitive>& primitives,
    std::shared_ptr<Primitive>&    out_infiniteSphere);

// implement in core/instantiator/primitiveInstantiator.cpp
void makePrimitive(
    const std::shared_ptr<SdData>&                   data,
    const StringKeyMap<Bsdf>&                        bsdfs,
    std::vector<std::shared_ptr<Intersector>>* const out_intersectors,
    StringKeyMap<Primitive>* const                   out_primitives);

} // namespace cadise::instantiator