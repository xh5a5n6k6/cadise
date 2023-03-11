#pragma once

#include "Core/Estimator/RadianceEstimator.h"

namespace cadise 
{

/*
     VPTEstimator: Vanilla Path Tracing Estimator
     
     This estimator implements vanilla path tracing.
     It means a path has contribution only when it hits an emitter. 
     It may be the most fundamental global illumination technique yet 
     still unbiased, so it is a good reference for ground truth rendering.
*/
class VPTEstimator : public RadianceEstimator 
{
public:
    explicit VPTEstimator(const int32 maxDepth);

    void estimate(
        const Scene&    scene, 
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    int32 _maxDepth;
};

// header implementation

inline std::string VPTEstimator::toString() const 
{
    return "VPT Estimator";
}

} // namespace cadise