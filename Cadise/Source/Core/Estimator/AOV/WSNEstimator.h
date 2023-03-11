#pragma once

#include "Core/Estimator/AOV/AOVEstimator.h"

namespace cadise
{

/*
    WSNEstimator: World-Space Normal Estimator
*/
class WSNEstimator : public AOVEstimator
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

inline std::string WSNEstimator::toString() const 
{
    return "AOV-WSN Estimator";
}

} // namespace cadise