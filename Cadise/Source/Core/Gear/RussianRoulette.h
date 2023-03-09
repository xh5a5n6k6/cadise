#pragma once

#include "core/spectrum/spectrum.h"

namespace cadise
{

class RussianRoulette 
{
public:
    static bool isSurvivedOnNextRound(
        const Spectrum& weight,
        Spectrum* const out_newWeight);
};

} // namespace cadise