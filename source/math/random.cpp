#include "math/random.h"

#include "math/math.h"

#include <random>

namespace cadise {

static std::random_device randomDevice;
static std::default_random_engine generator = std::default_random_engine(randomDevice());

real Random::nextReal() {
    static thread_local std::uniform_real_distribution<real> distribution(0.0_r, 1.0_r);

    return distribution(generator);
}

std::size_t Random::nextIndex(const std::size_t minIndex, const std::size_t maxIndex) {
    const std::size_t interval    = maxIndex - minIndex;
    const std::size_t sampleIndex = static_cast<std::size_t>(minIndex + nextReal() * interval);

    return math::clamp(sampleIndex, minIndex, maxIndex - 1);
}

} // namespace cadise