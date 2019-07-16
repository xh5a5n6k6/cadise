#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise {

class SurfaceGeometryInfo;

class Light {
public:
    virtual Spectrum evaluateSampleRadiance(Vector3R& lightDirection, const SurfaceGeometryInfo& surfaceGeometryInfo, real& t, real& pdf) const = 0;
    virtual Spectrum color() const = 0;
};

} // namespace cadise