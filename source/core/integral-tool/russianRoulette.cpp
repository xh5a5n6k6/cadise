#include "core/integral-tool/russianRoulette.h"

#include "math/random.h"

#include <algorithm>

namespace cadise {

namespace russianRoulette {

Spectrum weightOnNextPath(const Spectrum& weight) {
    real q = std::clamp(1.0_r - weight.maxComponent(), 0.05_r, 1.0_r);
    real randomNumber = random::get1D();

    // roulette survive
    if (randomNumber > q) {
        return weight / (1.0_r - q);
    }
    else {
        return Spectrum(0.0_r);
    }
}

} // namespace russianRoulette

} // namespace cadise