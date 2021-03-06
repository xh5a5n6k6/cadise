#pragma once

#include "core/estimator/radianceEstimator.h"

#include "core/renderer/photon-mapping/photonMap.h"

namespace cadise {

class VpmEstimator : public RadianceEstimator {
public:
    VpmEstimator(
        const PhotonMap* const photonMap,
        const std::size_t      numPhotonPaths,
        const real             searchRadius);

    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    const PhotonMap* _photonMap;

    // kernelFactor is the pre-calculated normalization term 
    // used in photon radiance estimation
    real _kernelFactor;
    real _searchRadius;
};

// header implementation

inline std::string VpmEstimator::toString() const {
    return "VPM Estimator";
}

} // namespace cadise