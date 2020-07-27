#pragma once

#include "core/integrator/integrator.h"

namespace cadise {

/*
    WhittedIntegrator is Whitted-style ray tracing integrator.

    It considers direct lighting and reflection/refraction effects.
    (reflection/refraction effects are calculated only at specular surface)
*/
class WhittedIntegrator : public Integrator {
public:
    explicit WhittedIntegrator(const int32 maxDepth);

    void traceRadiance(
        const Scene&    scene, 
        const Ray&      ray,
        Spectrum* const out_radiance) const override;

    std::string toString() const override;

private:
    int32 _maxDepth;
};

// header implementation

inline std::string WhittedIntegrator::toString() const {
    return "Whitted Integrator";
}

} // namespace cadise