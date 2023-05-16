#pragma once

#include "Core/Sampler/Sampler.h"

namespace cadise
{

class RandomSampler : public Sampler
{
public:
    explicit RandomSampler(const std::size_t sampleNumber);

    std::unique_ptr<Sampler> clone(const std::size_t sampleNumber) const override;

    std::unique_ptr<SampleRecord1D> requestSample1D() const override;
    std::unique_ptr<SampleRecord2D> requestSample2D() const override;
};

} // namespace cadise