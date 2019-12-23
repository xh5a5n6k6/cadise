#pragma once

#include "core/integral-tool/misMode.h"
#include "fundamental/assertion.h"
#include "math/type/fundamentalType.h"

namespace cadise {

template<MisMode Mode>
class Mis {
public:
    static real weight(const real  pdfA, 
                       const real  pdfB,
                       const int32 nA = 1, 
                       const int32 nB = 1);
};

// template header implementation

template<MisMode Mode>
real Mis<Mode>::weight(const real  pdfA,
                       const real  pdfB,
                       const int32 nA,
                       const int32 nB) {

    CADISE_ASSERT_GE(pdfA, 0.0_r);
    CADISE_ASSERT_GE(pdfB, 0.0_r);

    const real mulA = nA * pdfA;
    const real mulB = nB * pdfB;

    if constexpr (Mode == MisMode::BALANCE) {
        return mulA / (mulA + mulB);
    }
    // using power as 2
    else if constexpr (Mode == MisMode::POWER) {
        const real mulA2 = mulA * mulA;
        const real mulB2 = mulB * mulB;

        return mulA2 / (mulA2 + mulB2);
    }
    else {
        return 0.0_r;
    }
}

} // namespace cadise