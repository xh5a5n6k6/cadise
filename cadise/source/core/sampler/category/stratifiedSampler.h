#pragma once

#include "core/sampler/sampler.h"

namespace cadise
{

class StratifiedSampler : public Sampler
{
public:
    explicit StratifiedSampler(const std::size_t sampleNumber);

    std::unique_ptr<Sampler> clone(const std::size_t sampleNumber) const override;

    std::unique_ptr<SampleRecord1D> requestSample1D() const override;
    std::unique_ptr<SampleRecord2D> requestSample2D() const override;

private:
    std::size_t _baseNumber;
};

} // namespace cadise