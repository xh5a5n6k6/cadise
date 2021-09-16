#pragma once

#include "core/estimator/aov/aovEstimator.h"

namespace cadise 
{

/*
    UvwEstimator: UVs Estimator
*/
class UvwEstimator : public AovEstimator 
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

inline std::string UvwEstimator::toString() const 
{
    return "AOV-UVW Estimator";
}

} // namespace cadise