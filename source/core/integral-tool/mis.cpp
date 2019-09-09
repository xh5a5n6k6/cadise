#include "core/integral-tool/mis.h"

namespace cadise {

namespace mis {

real blanceHeuristic(const real pdfA, const real pdfB) {
    return pdfA / (pdfA + pdfB);
}

real powerHeuristic(const real pdfA, const real pdfB) {
    return (pdfA * pdfA) / (pdfA * pdfA + pdfB * pdfB);
}

} // namespace mis

} // namespace cadise