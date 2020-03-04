#include "core/surface/fresnel/conductorFresnel.h"

namespace cadise {

ConductorFresnel::ConductorFresnel(
    const real      iorOuter, 
    const Spectrum& eta, 
    const Spectrum& k) :

    _iorOuter(iorOuter),
    _eta(eta),
    _k(k) {
}

} // namespace cadise