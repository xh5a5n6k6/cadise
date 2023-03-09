#pragma once

#include "core/estimator/aov/aovEstimator.h"

namespace cadise
{

/*
    WsnEstimator: World-Space Normal Estimator
*/
class WsnEstimator : public AovEstimator
{
public:
    using AovEstimator::AovEstimator;

    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;
};

// header implementation

inline std::string WsnEstimator::toString() const 
{
    return "AOV-WSN Estimator";
}

} // namespace cadise