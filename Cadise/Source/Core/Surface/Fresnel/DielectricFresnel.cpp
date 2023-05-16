#include "Core/Surface/Fresnel/DielectricFresnel.h"

namespace cadise
{

DielectricFresnel::DielectricFresnel(const real iorOuter, const real iorInner) :
    _iorOuter(iorOuter),
    _iorInner(iorInner)
{}

real DielectricFresnel::iorOuter() const
{
    return _iorOuter;
}

real DielectricFresnel::iorInner() const
{
    return _iorInner;
}

} // namespace cadise