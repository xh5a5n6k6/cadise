#pragma once

#include "Core/Sampler/SampleRecord.h"

namespace cadise
{

class SampleRecord1D : public SampleRecord
{
public:
    explicit SampleRecord1D(const std::size_t sampleNumber);

    void shuffle() override;

    real nextSample();
};

} // namespace cadise