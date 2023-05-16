#pragma once

#include "Core/Spectrum/Spectrum.h"

#include <string>

// forward declaration
namespace cadise
{
    class Ray;
    class Scene;
}

namespace cadise
{

class EnergyEstimator
{
public:
    virtual ~EnergyEstimator();

    virtual void estimate(
        const Scene&    scene,
        const Ray&      ray,
        Spectrum* const out_radiance) const = 0;

    // It is used to check if this estimation
    // needs color space transformation.
    //
    // for example, AOV related estimators use
    // evaluated data directly, such as normal
    // and uvw.
    virtual bool useDirectly() const = 0;

    virtual std::string toString() const = 0;
};

} // namespace cadise