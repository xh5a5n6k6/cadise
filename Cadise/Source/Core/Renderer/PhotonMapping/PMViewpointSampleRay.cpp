#include "Core/Renderer/PhotonMapping/PMViewpointSampleRay.h"

namespace cadise 
{

PMViewpointSampleRay::PMViewpointSampleRay() = default;

PMViewpointSampleRay::PMViewpointSampleRay(
    const Ray&      sampleRay,
    const Spectrum& pathThroughput,
    const int32     depth) :

    _sampleRay(sampleRay),
    _pathThroughput(pathThroughput),
    _depth(depth)
{}

const Ray& PMViewpointSampleRay::sampleRay() const
{
    return _sampleRay;
}

const Spectrum& PMViewpointSampleRay::pathThroughput() const
{
    return _pathThroughput;
}

int32 PMViewpointSampleRay::depth() const
{
    return _depth;
}

} // namespace cadise