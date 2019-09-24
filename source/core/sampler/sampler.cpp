#include "core/sampler/sampler.h"

namespace cadise {

Sampler::Sampler(const std::size_t sampleNumber) :
    _sampleNumber(sampleNumber) {
}

std::size_t Sampler::sampleNumber() const {
    return _sampleNumber;
}

} // namespace cadise