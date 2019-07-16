#include "math/random.h"

#include <random>

namespace cadise {

namespace random {

std::random_device rd;
std::default_random_engine gen = std::default_random_engine(rd());

real get1D() {
    std::uniform_real_distribution<real> dis(0.0_r, 1.0_r);

    return dis(gen);
}

Vector2R get2D() {
    return Vector2R(get1D(), get1D());
}

} // namespace random

} // namespace cadise