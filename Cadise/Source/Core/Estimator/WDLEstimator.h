#pragma once

#include "Core/Estimator/RadianceEstimator.h"

namespace cadise
{

/*
    WDLEstimator: Whitted-Style Direct Light Estimator

    This estimator considers direct lighting and reflection/refraction effects.
    (reflection/refraction effects are calculated only at specular surface)
*/
class WDLEstimator : public RadianceEstimator
{
public:
    explicit WDLEstimator(const int32 maxDepth);

    void estimate(
        const Scene&    scene, 
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    int32 _maxDepth;
};

// header implementation

inline std::string WDLEstimator::toString() const
{
    return "WDL Estimator";
}

} // namespace cadise