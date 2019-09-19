#pragma once

#include "math/fundamentalType.h"

namespace cadise {

class SampleRecord {
public:
    SampleRecord(const std::size_t sampleNumber);

    virtual void shuffle() = 0;

    void setData(const std::size_t offset, const real value);

    void allocate(std::size_t dimension);
    void clear();

protected:
    real* _data;

    std::size_t _sampleNumber;
    std::size_t _currentSampleIndex;
};

} // namespace cadise