#include "math/random.h"

#include <random>

namespace cadise {

namespace random {

static std::random_device rd;
static std::default_random_engine gen = std::default_random_engine(rd());

real get1D() {
    std::uniform_real_distribution<real> dis(0.0_r, 1.0_r);

    return dis(gen);
}

Vector2R get2D() {
    return Vector2R(get1D(), get1D());
}

int32 get1DInt32(const int32 minNumber, const int32 maxNumber) {
    std::uniform_int_distribution<int32> dis(minNumber, maxNumber);

    return dis(gen);
}

} // namespace random

} // namespace cadise