#pragma once

#include "Math/TVector3.h"

namespace cadise
{

class DifferentialGeometry
{
public:
    DifferentialGeometry();
    DifferentialGeometry(
        const Vector3R& dPdU,
        const Vector3R& dPdV,
        const Vector3R& dNdU,
        const Vector3R& dNdV);

    const Vector3R& dPdU() const;
    const Vector3R& dPdV() const;
    const Vector3R& dNdU() const;
    const Vector3R& dNdV() const;

private:
    Vector3R _dPdU;
    Vector3R _dPdV;
    Vector3R _dNdU;
    Vector3R _dNdV;
};

} // namespace cadise