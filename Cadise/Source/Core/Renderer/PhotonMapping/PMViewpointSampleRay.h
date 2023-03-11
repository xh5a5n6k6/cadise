#pragma once

#include "Core/Ray.h"
#include "Core/Spectrum/Spectrum.h"

namespace cadise
{

class PMViewpointSampleRay
{
public:
    PMViewpointSampleRay();
    PMViewpointSampleRay(
        const Ray&      sampleRay,
        const Spectrum& pathThroughput,
        const int32     depth);

    const Ray& sampleRay() const;
    const Spectrum& pathThroughput() const;
    int32 depth() const;

private:
    Ray      _sampleRay;
    Spectrum _pathThroughput;
    int32    _depth;
};

} // namespace cadise