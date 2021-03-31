#pragma once

#include "math/tVector3.h"

namespace cadise {

template<typename T>
class TLocalCoordinateSystem3 {
public:
    TLocalCoordinateSystem3();
    TLocalCoordinateSystem3(
        const TVector3<T>& xAxis,
        const TVector3<T>& yAxis,
        const TVector3<T>& zAxis);

    void initializeViaUnitY(const TVector3<T>& unitY);

    TVector3<T> worldToLocal(const TVector3<T>& unitVector) const;
    TVector3<T> localToWorld(const TVector3<T>& unitVector) const;

    T cosTheta(const TVector3<T>& unitVector) const;
    T cos2Theta(const TVector3<T>& unitVector) const;

    T sinTheta(const TVector3<T>& unitVector) const;
    T sin2Theta(const TVector3<T>& unitVector) const;

    T tanTheta(const TVector3<T>& unitVector) const;
    T tan2Theta(const TVector3<T>& unitVector) const;

    T cosPhi(const TVector3<T>& unitVector) const;
    T cos2Phi(const TVector3<T>& unitVector) const;

    T sinPhi(const TVector3<T>& unitVector) const;
    T sin2Phi(const TVector3<T>& unitVector) const;

    T tanPhi(const TVector3<T>& unitVector) const;
    T tan2Phi(const TVector3<T>& unitVector) const;

    const TVector3<T>& xAxis() const;
    const TVector3<T>& yAxis() const;
    const TVector3<T>& zAxis() const;

    void setAxes(
        const TVector3<T>& xAxis,
        const TVector3<T>& yAxis,
        const TVector3<T>& zAxis);

    void setXAxis(const TVector3<T>& xAxis);
    void setYAxis(const TVector3<T>& yAxis);
    void setZAxis(const TVector3<T>& zAxis);

private:
    TVector3<T> _xAxis;
    TVector3<T> _yAxis;
    TVector3<T> _zAxis;
};

} // namespace cadise

#include "math/tLocalCoordinateSystem3.ipp"