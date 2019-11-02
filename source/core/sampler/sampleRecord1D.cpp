#include "core/sampler/sampleRecord1D.h"

#include "fundamental/assertion.h"

#include "math/math.h"
#include "math/random.h"

namespace cadise {

SampleRecord1D::SampleRecord1D(const std::size_t sampleNumber) :
    SampleRecord(sampleNumber) {
    
    allocate(1);
}

void SampleRecord1D::shuffle() {
    for (std::size_t i = 0; i < _sampleNumber; ++i) {
        const std::size_t randomIndex = random::nextIndex(i, _sampleNumber - 1);
        math::swap(_data[i], _data[randomIndex]);
    }
}

real SampleRecord1D::nextSample() {
    CADISE_ASSERT_LT(_currentSampleIndex, _sampleNumber);

    std::size_t indexOffset = _currentSampleIndex;
    _currentSampleIndex += 1;

    return _data[indexOffset];
}

} // namespace cadise