#pragma once

#include "Core/Estimator/RadianceEstimator.h"

namespace cadise
{

/*
     PTEstimator: Path Tracing Estimator

     This estimator implements vanilla path tracing plus next event estimation.

     Jacco Bikker has a great tutorial about variance reduction at his lecture 
     such as NEE and MIS used here.
     Reference Note:
     http://www.cs.uu.nl/docs/vakken/magr/portfolio/INFOMAGR/lecture8.pdf
*/
class PTEstimator : public RadianceEstimator
{
public:
    explicit PTEstimator(const int32 maxPathLength);

    void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    int32 _maxPathLength;
};

// header implementation

inline std::string PTEstimator::toString() const
{
    return "PT Estimator";
}

} // namespace cadise