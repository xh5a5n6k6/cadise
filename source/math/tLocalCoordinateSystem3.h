#pragma once

#include "math/tVector.h"

namespace cadise {

template<typename T>
class TLocalCoordinateSystem3 {
public:
    TLocalCoordinateSystem3();
    TLocalCoordinateSystem3(
        const TVector<T, 3>& xAxis,
        const TVector<T, 3>& yAxis,
        const TVector<T, 3>& zAxis);

    void initializeViaUnitY(const TVector<T, 3>& unitY);

    TVector<T, 3> worldToLocal(const TVector<T, 3>& unitVector) const;
    TVector<T, 3> localToWorld(const TVector<T, 3>& unitVector) const;

    T cosTheta(const TVector<T, 3>& unitVector) const;
    T cos2Theta(const TVector<T, 3>& unitVector) const;

    T sinTheta(const TVector<T, 3>& unitVector) const;
    T sin2Theta(const TVector<T, 3>& unitVector) const;

    T tanTheta(const TVector<T, 3>& unitVector) const;
    T tan2Theta(const TVector<T, 3>& unitVector) const;

    T cosPhi(const TVector<T, 3>& unitVector) const;
    T cos2Phi(const TVector<T, 3>& unitVector) const;

    T sinPhi(const TVector<T, 3>& unitVector) const;
    T sin2Phi(const TVector<T, 3>& unitVector) const;

    T tanPhi(const TVector<T, 3>& unitVector) const;
    T tan2Phi(const TVector<T, 3>& unitVector) const;

    const TVector<T, 3>& xAxis() const;
    const TVector<T, 3>& yAxis() const;
    const TVector<T, 3>& zAxis() const;

    void setAxes(
        const TVector<T, 3>& xAxis,
        const TVector<T, 3>& yAxis,
        const TVector<T, 3>& zAxis);

    void setXAxis(const TVector<T, 3>& xAxis);
    void setYAxis(const TVector<T, 3>& yAxis);
    void setZAxis(const TVector<T, 3>& zAxis);

private:
    TVector<T, 3> _xAxis;
    TVector<T, 3> _yAxis;
    TVector<T, 3> _zAxis;
};

} // namespace cadise

#include "math/tLocalCoordinateSystem3.ipp"