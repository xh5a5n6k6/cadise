#pragma once

#include "core/sampler/sampleRecord.h"

#include "math/mathType.h"

namespace cadise {

class SampleRecord2D : public SampleRecord {
public:
    SampleRecord2D(const std::size_t sampleNumber);

    void shuffle() override;

    Vector2R nextSample();
};

} // namespace cadise