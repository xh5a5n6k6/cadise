#pragma once

#include "core/estimator/energyEstimator.h"

namespace cadise {

class AovEstimator : public EnergyEstimator {
public:
    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const override = 0;

    bool useDirectly() const override;

    std::string toString() const override = 0;
};

} // namespace cadise