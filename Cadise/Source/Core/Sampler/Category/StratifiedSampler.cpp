#include "Core/Sampler/Category/StratifiedSampler.h"

#include "Core/Sampler/SampleRecord1D.h"
#include "Core/Sampler/SampleRecord2D.h"
#include "Math/Math.h"
#include "Math/Random.h"

#include <cmath>

namespace cadise
{

StratifiedSampler::StratifiedSampler(const std::size_t sampleNumber) :
    Sampler(sampleNumber)
{
    const std::size_t nearestSquareNumber = math::nearest_lower_square_number(sampleNumber);
    _baseNumber = static_cast<std::size_t>(std::sqrt(nearestSquareNumber));
}

std::unique_ptr<Sampler> StratifiedSampler::clone(const std::size_t sampleNumber) const
{
    return std::make_unique<StratifiedSampler>(sampleNumber);
}

std::unique_ptr<SampleRecord1D> StratifiedSampler::requestSample1D() const
{
    std::unique_ptr<SampleRecord1D> sample1D = std::make_unique<SampleRecord1D>(_sampleNumber);

    const real dx = 1.0_r / static_cast<real>(_sampleNumber);

    for (std::size_t i = 0; i < _sampleNumber; ++i)
    {
        const real jitter = Random::nextReal();

        sample1D->setData(i, (static_cast<real>(i) + jitter) * dx);
    }

    sample1D->shuffle();
    return std::move(sample1D);
}

std::unique_ptr<SampleRecord2D> StratifiedSampler::requestSample2D() const
{
    std::unique_ptr<SampleRecord2D> sample2D = std::make_unique<SampleRecord2D>(_sampleNumber);

    std::size_t index = 0;
    const std::size_t strataNumber = _baseNumber * _baseNumber;
    const real dx = 1.0_r / static_cast<real>(_baseNumber);
    const real dy = 1.0_r / static_cast<real>(_baseNumber);

    for (std::size_t iy = 0; iy < _baseNumber; ++iy)
    {
        for (std::size_t ix = 0; ix < _baseNumber; ++ix)
        {
            const std::size_t indexOffset = 2 * index;
            const real jitterX = Random::nextReal();
            const real jitterY = Random::nextReal();

            sample2D->setData(indexOffset, (static_cast<real>(ix) + jitterX) * dx);
            sample2D->setData(indexOffset + 1, (static_cast<real>(iy) + jitterY) * dy);

            index += 1;
        }
    }

    std::size_t remainSample = _sampleNumber - strataNumber;
    if (remainSample > 0)
    {
        for (std::size_t i = 0; i < remainSample; ++i)
        {
            const std::size_t indexOffset = 2 * index;

            sample2D->setData(indexOffset, Random::nextReal());
            sample2D->setData(indexOffset + 1, Random::nextReal());

            index += 1;
        }
    }

    sample2D->shuffle();
    return std::move(sample2D);
}

} // namespace cadise