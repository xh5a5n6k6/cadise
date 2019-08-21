#pragma once

#include "core/integrator/integrator.h"

namespace cadise {

// PathIntegrator means original path integrator plus next event estimation.
//
// Jacco Bikker has a great tutorial about variance reduction at his lecture 
// such as NEE and MIS used in PathIntegrator.
// Reference : http://www.cs.uu.nl/docs/vakken/magr/portfolio/INFOMAGR/lecture8.pdf
//
class PathIntegrator : public Integrator {
public:
    PathIntegrator(const int32 maxDepth);

    Spectrum traceRadiance(const Scene& scene, Ray& ray) const;

private:
    int32 _maxDepth;
};

} // namespace cadise