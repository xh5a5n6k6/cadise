#include "core/integral-tool/mis.h"

namespace cadise {

namespace mis {

real blanceHeuristic(const real pdf1, const real pdf2) {
    return pdf1 / (pdf1 + pdf2);
}

real powerHeuristic(const real pdf1, const real pdf2) {
    return (pdf1 * pdf1) / (pdf1 * pdf1 + pdf2 * pdf2);
}

} // namespace mis

} // namespace cadise