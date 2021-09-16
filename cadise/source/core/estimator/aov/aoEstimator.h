#pragma once

#include "core/estimator/aov/aovEstimator.h"

namespace cadise 
{

/*
    AoEstimator: Ambient Occlusion Estimator

    This estimator implements an approximate method for 
    evaluating global illumination. It is preferred to use 
    this estimator at outdoor scene.
*/
class AoEstimator : public AovEstimator 
{
public:
    AoEstimator();
    explicit AoEstimator(const std::size_t numSampleRays);

    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    std::size_t _numSampleRays;
};

// header implementation

inline std::string AoEstimator::toString() const 
{
    return "AOV-AO Estimator";
}

} // namespace cadise