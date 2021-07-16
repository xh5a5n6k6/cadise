#include "core/renderer/photon-mapping/pmViewpointSampleRay.h"

namespace cadise {

PmViewpointSampleRay::PmViewpointSampleRay() = default;

PmViewpointSampleRay::PmViewpointSampleRay(
    const Ray&      sampleRay,
    const Spectrum& pathThroughput,
    const int32     depth) :

    _sampleRay(sampleRay),
    _pathThroughput(pathThroughput),
    _depth(depth) {
}

const Ray& PmViewpointSampleRay::sampleRay() const {
    return _sampleRay;
}

const Spectrum& PmViewpointSampleRay::pathThroughput() const {
    return _pathThroughput;
}

const int32 PmViewpointSampleRay::depth() const {
    return _depth;
}

} // namespace cadise