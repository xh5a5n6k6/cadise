#pragma once

#include "core/ray.h"
#include "core/spectrum/spectrum.h"

namespace cadise
{

class PmViewpointSampleRay
{
public:
    PmViewpointSampleRay();
    PmViewpointSampleRay(
        const Ray&      sampleRay,
        const Spectrum& pathThroughput,
        const int32     depth);

    const Ray& sampleRay() const;
    const Spectrum& pathThroughput() const;
    const int32 depth() const;

private:
    Ray      _sampleRay;
    Spectrum _pathThroughput;
    int32    _depth;
};

} // namespace cadise