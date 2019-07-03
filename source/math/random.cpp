#include "math/random.h"

#include <random>

namespace cadise {

namespace random {

std::random_device rd;
std::default_random_engine gen = std::default_random_engine(rd());

Vector2R get2D() {
    std::uniform_real_distribution<real> disX(0.0_r, 1.0_r);
    std::uniform_real_distribution<real> disY(0.0_r, 1.0_r);

    return Vector2R(disX(gen), disY(gen));
}

} // namespace random

} // namespace cadise