#pragma once

#include "core/estimator/radianceEstimator.h"

namespace cadise
{

/*
     PtEstimator: Path Tracing Estimator

     This estimator implements vanilla path tracing plus next event estimation.

     Jacco Bikker has a great tutorial about variance reduction at his lecture 
     such as NEE and MIS used here.
     Reference Note:
     http://www.cs.uu.nl/docs/vakken/magr/portfolio/INFOMAGR/lecture8.pdf
*/
class PtEstimator : public RadianceEstimator
{
public:
    explicit PtEstimator(const int32 maxDepth);

    void estimate(
        const Scene&    scene, 
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    int32 _maxDepth;
};

// header implementation

inline std::string PtEstimator::toString() const 
{
    return "PT Estimator";
}

} // namespace cadise