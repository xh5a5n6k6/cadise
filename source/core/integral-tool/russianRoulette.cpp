#include "core/integral-tool/russianRoulette.h"

#include "math/math.h"
#include "math/random.h"

namespace cadise {

namespace russianRoulette {

Spectrum weightOnNextPath(const Spectrum& weight) {
    real q = math::clamp(1.0_r - weight.maxComponent(), 0.05_r, 1.0_r);
    real randomNumber = random::nextReal();

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