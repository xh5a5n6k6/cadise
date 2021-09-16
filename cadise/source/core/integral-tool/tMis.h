#pragma once

#include "core/integral-tool/eMisMode.h"
#include "fundamental/assertion.h"
#include "math/type/fundamentalType.h"

namespace cadise 
{

template<EMisMode Mode>
class TMis
{
public:
    real weight(
        const real  pdfA, 
        const real  pdfB,
        const int32 nA = 1, 
        const int32 nB = 1);
};

// template header implementation

template<EMisMode Mode>
inline real TMis<Mode>::weight(
    const real  pdfA,
    const real  pdfB,
    const int32 nA,
    const int32 nB)
{
    CS_ASSERT_GE(pdfA, 0.0_r);
    CS_ASSERT_GE(pdfB, 0.0_r);

    const real mulA = nA * pdfA;
    const real mulB = nB * pdfB;

    if constexpr (Mode == EMisMode::BALANCE)
    {
        return mulA / (mulA + mulB);
    }
    // using power as 2
    else if constexpr (Mode == EMisMode::POWER)
    {
        const real mulA2 = mulA * mulA;
        const real mulB2 = mulB * mulB;

        return mulA2 / (mulA2 + mulB2);
    }
    else 
    {
        return 0.0_r;
    }
}

} // namespace cadise