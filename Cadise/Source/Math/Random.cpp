#include "Math/Random.h"

#include <atomic>
#include <random>

namespace cadise
{

static std::atomic<int32> seed = 81;

real Random::nextReal()
{
    static thread_local std::random_device randomDevice;
    static thread_local std::mt19937 generator(seed += randomDevice());

    std::uniform_real_distribution<real> distribution(0.0_r, 1.0_r);

    return distribution(generator);
}

std::size_t Random::nextIndex(
    const std::size_t minIndex, 
    const std::size_t maxIndex) 
{
    const std::size_t interval    = maxIndex - minIndex;
    const std::size_t sampleIndex = static_cast<std::size_t>(minIndex + nextReal() * interval);

    return 
        (sampleIndex < minIndex) ? minIndex : 
        (sampleIndex > maxIndex - 1) ? maxIndex - 1 : sampleIndex;
}

} // namespace cadise