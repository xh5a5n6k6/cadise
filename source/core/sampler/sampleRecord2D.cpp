#include "core/sampler/sampleRecord2D.h"

#include "fundamental/assertion.h"

#include "math/math.h"
#include "math/random.h"
#include "math/vector.h"

namespace cadise {

SampleRecord2D::SampleRecord2D(const std::size_t sampleNumber) :
    SampleRecord(sampleNumber) {

    allocate(2);
}

void SampleRecord2D::shuffle() {
    for (std::size_t i = 0; i < _sampleNumber; i++) {
        const std::size_t randomIndex = random::nextIndex(i, _sampleNumber - 1);
        
        for (std::size_t id = 0; id < 2; id++) {
            const std::size_t indexOffset       = 2 * i;
            const std::size_t randomIndexOffset = 2 * randomIndex;
            math::swap(_data[indexOffset + id], _data[randomIndexOffset + id]);
        }
    }
}

Vector2R SampleRecord2D::nextSample() {
    CADISE_ASSERT_LT(_currentSampleIndex, _sampleNumber);

    std::size_t indexOffset = 2 * _currentSampleIndex;
    _currentSampleIndex += 1;

    return Vector2R(_data[indexOffset], _data[indexOffset + 1]);
}

} // namespace cadise