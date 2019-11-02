#include "core/sampler/category/randomSampler.h"

#include "core/sampler/sampleRecord1D.h"
#include "core/sampler/sampleRecord2D.h"

#include "math/random.h"

namespace cadise {

RandomSampler::RandomSampler(const std::size_t sampleNumber) :
    Sampler(sampleNumber) {
}

std::unique_ptr<Sampler> RandomSampler::clone(const std::size_t sampleNumber) const {
    return std::make_unique<RandomSampler>(sampleNumber);
}

std::unique_ptr<SampleRecord1D> RandomSampler::requestSample1D() const {
    std::unique_ptr<SampleRecord1D> sample1D = std::make_unique<SampleRecord1D>(_sampleNumber);
    
    for (std::size_t i = 0; i < _sampleNumber; ++i) {
        sample1D->setData(i, random::nextReal());
    }

    return std::move(sample1D);
}

std::unique_ptr<SampleRecord2D> RandomSampler::requestSample2D() const {
    std::unique_ptr<SampleRecord2D> sample2D = std::make_unique<SampleRecord2D>(_sampleNumber);

    for (std::size_t i = 0; i < _sampleNumber; ++i) {
        const std::size_t indexOffset = 2 * i;

        sample2D->setData(indexOffset, random::nextReal());
        sample2D->setData(indexOffset + 1, random::nextReal());
    }

    return std::move(sample2D);
}

} // namespace cadise