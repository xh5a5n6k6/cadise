#pragma once

#include "core/renderer/pm-renderer/photonMap.h"
#include "core/spectrum/spectrum.h"

namespace cadise {

class Ray;
class Scene;

class PmRadianceEstimator {
public:
    PmRadianceEstimator(
        const PhotonMap* const photonMap,
        const std::size_t      numPhotonPaths);

    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        const real      searchRadius,
        Spectrum* const out_radiance) const;

private:
    const PhotonMap*  _photonMap;
    
    std::size_t _numPhotonPaths;
};

} // namespace cadise