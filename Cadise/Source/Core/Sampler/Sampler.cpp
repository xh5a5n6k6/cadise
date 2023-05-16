#include "Core/Sampler/Sampler.h"

namespace cadise
{

Sampler::Sampler(const std::size_t sampleNumber) :
    _sampleNumber(sampleNumber)
{}

Sampler::~Sampler() = default;

std::size_t Sampler::sampleNumber() const
{
    return _sampleNumber;
}

} // namespace cadise