#include "core/intersector/differentialGeometry.h"

namespace cadise {

DifferentialGeometry::DifferentialGeometry() = default;

DifferentialGeometry::DifferentialGeometry(
    const Vector3R& dPdU,
    const Vector3R& dPdV,
    const Vector3R& dNdU,
    const Vector3R& dNdV) :

    _dPdU(dPdU),
    _dPdV(dPdV),
    _dNdU(dNdU),
    _dNdV(dNdV) {
}

const Vector3R& DifferentialGeometry::dPdU() const {
    return _dPdU;
}

const Vector3R& DifferentialGeometry::dPdV() const {
    return _dPdV;
}

const Vector3R& DifferentialGeometry::dNdU() const {
    return _dNdU;
}

const Vector3R& DifferentialGeometry::dNdV() const {
    return _dNdV;
}

} // namespace cadise