#include "core/integral-tool/russianRoulette.h"

#include "fundamental/assertion.h"
#include "math/math.h"
#include "math/random.h"

namespace cadise {

bool RussianRoulette::isSurvivedOnNextRound(
    const Spectrum& weight,
    Spectrum* const out_newWeight) {
    
    CADISE_ASSERT(out_newWeight);

    const real q      = math::clamp(1.0_r - weight.maxComponent(), 0.05_r, 1.0_r);
    const real sample = Random::nextReal();

    // it survives on next round
    if (sample > q) {
        const real weightScale = 1.0_r / (1.0_r - q);
        out_newWeight->set(weight.mul(weightScale));

        return true;
    }
    else {
        return false;
    }
}

} // namespace cadise