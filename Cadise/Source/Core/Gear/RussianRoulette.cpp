#include "Core/Gear/RussianRoulette.h"

#include "Foundation/Assertion.h"
#include "Math/Math.h"
#include "Math/Random.h"

namespace cadise 
{

bool RussianRoulette::isSurvivedOnNextRound(
    const Spectrum& weight,
    Spectrum* const out_newWeight)
{
    CS_ASSERT(out_newWeight);

    const real q      = math::clamp(1.0_r - weight.maxComponent(), 0.05_r, 1.0_r);
    const real sample = Random::nextReal();

    // it survives on next round
    if (sample > q) 
    {
        const real weightScale = 1.0_r / (1.0_r - q);
        out_newWeight->set(weight.mul(weightScale));

        return true;
    }
    else 
    {
        return false;
    }
}

} // namespace cadise