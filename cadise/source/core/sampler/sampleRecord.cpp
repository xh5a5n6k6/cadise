#include "core/sampler/sampleRecord.h"

namespace cadise 
{

SampleRecord::SampleRecord(const std::size_t sampleNumber) :
    _data(nullptr),
    _sampleNumber(sampleNumber),
    _currentSampleIndex(0)
{}

SampleRecord::~SampleRecord() = default;

void SampleRecord::setData(const std::size_t offset, const real value) 
{
    _data[offset] = value;
}

void SampleRecord::allocate(std::size_t dimension) 
{
    _data = new real[dimension * _sampleNumber];
}

void SampleRecord::clear()
{
    delete[] _data;
}

} // namespace cadise