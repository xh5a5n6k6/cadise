#pragma once

#include "Core/Estimator/AOV/AOVEstimator.h"

namespace cadise 
{

/*
    UVWEstimator: UVs Estimator
*/
class UVWEstimator : public AOVEstimator 
{
public:
    using AOVEstimator::AOVEstimator;

    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;
};

// header implementation

inline std::string UVWEstimator::toString() const 
{
    return "AOV-UVW Estimator";
}

} // namespace cadise