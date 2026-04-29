#pragma once

#include "Core/Spectrum/Spectrum.h"

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