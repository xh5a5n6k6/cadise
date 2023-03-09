#pragma once

#include "math/type/fundamentalType.h"

namespace cadise 
{

class SampleRecord
{
public:
    explicit SampleRecord(const std::size_t sampleNumber);

    virtual ~SampleRecord();

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