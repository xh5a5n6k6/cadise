#pragma once

#include "Core/Sampler/SampleRecord.h"

#include "Math/Type/MathType.h"

namespace cadise
{

class SampleRecord2D : public SampleRecord 
{
public:
    explicit SampleRecord2D(const std::size_t sampleNumber);

    void shuffle() override;

    Vector2R nextSample();
};

} // namespace cadise