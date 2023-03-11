#pragma once

#include "Core/Estimator/EnergyEstimator.h"

namespace cadise 
{

class AOVEstimator : public EnergyEstimator 
{
public:
    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const override = 0;

    bool useDirectly() const override;

    std::string toString() const override = 0;
};

} // namespace cadise