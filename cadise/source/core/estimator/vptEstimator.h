#pragma once

#include "core/estimator/radianceEstimator.h"

namespace cadise 
{

/*
     VptEstimator: Vanilla Path Tracing Estimator
     
     This estimator implements vanilla path tracing.
     It means a path has contribution only when it hits an emitter. 
     It may be the most fundamental global illumination technique yet 
     still unbiased, so it is a good reference for ground truth rendering.
*/
class VptEstimator : public RadianceEstimator 
{
public:
    explicit VptEstimator(const int32 maxDepth);

    void estimate(
        const Scene&    scene, 
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    int32 _maxDepth;
};

// header implementation

inline std::string VptEstimator::toString() const 
{
    return "VPT Estimator";
}

} // namespace cadise