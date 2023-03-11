#pragma once

#include "Core/Gear/EMisMode.h"
#include "Foundation/Assertion.h"
#include "Math/Type/PrimitiveType.h"

namespace cadise 
{

template<EMISMode Mode>
class TMIS
{
public:
    real weight(
        const real  pdfA, 
        const real  pdfB,
        const int32 nA = 1, 
        const int32 nB = 1) const;
};

// template header implementation

template<EMISMode Mode>
inline real TMIS<Mode>::weight(
    const real  pdfA,
    const real  pdfB,
    const int32 nA,
    const int32 nB) const
{
    CS_ASSERT_GE(pdfA, 0.0_r);
    CS_ASSERT_GE(pdfB, 0.0_r);

    const real mulA = nA * pdfA;
    const real mulB = nB * pdfB;

    if constexpr (Mode == EMISMode::Balance)
    {
        return mulA / (mulA + mulB);
    }
    // using power as 2
    else if constexpr (Mode == EMISMode::Power)
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