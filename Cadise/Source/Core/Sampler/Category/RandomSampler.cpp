#include "Core/Sampler/Category/RandomSampler.h"

#include "Core/Sampler/SampleRecord1D.h"
#include "Core/Sampler/SampleRecord2D.h"
#include "Math/Random.h"

namespace cadise
{

RandomSampler::RandomSampler(const std::size_t sampleNumber) :
    Sampler(sampleNumber)
{}

std::unique_ptr<Sampler> RandomSampler::clone(const std::size_t sampleNumber) const 
{
    return std::make_unique<RandomSampler>(sampleNumber);
}

std::unique_ptr<SampleRecord1D> RandomSampler::requestSample1D() const 
{
    std::unique_ptr<SampleRecord1D> sample1D = std::make_unique<SampleRecord1D>(_sampleNumber);
    
    for (std::size_t i = 0; i < _sampleNumber; ++i) 
    {
        sample1D->setData(i, Random::nextReal());
    }

    return std::move(sample1D);
}

std::unique_ptr<SampleRecord2D> RandomSampler::requestSample2D() const 
{
    std::unique_ptr<SampleRecord2D> sample2D = std::make_unique<SampleRecord2D>(_sampleNumber);

    for (std::size_t i = 0; i < _sampleNumber; ++i) 
    {
        const std::size_t indexOffset = 2 * i;

        sample2D->setData(indexOffset, Random::nextReal());
        sample2D->setData(indexOffset + 1, Random::nextReal());
    }

    return std::move(sample2D);
}

} // namespace cadise