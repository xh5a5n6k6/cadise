#pragma once

#include "math/tLocalCoordinateSystem3.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <cmath>
#include <limits>

namespace cadise {

template<typename T>
inline TLocalCoordinateSystem3<T>::TLocalCoordinateSystem3() :
    _xAxis(T(1), T(0), T(0)),
    _yAxis(T(0), T(1), T(0)),
    _zAxis(T(0), T(0), T(1)) {
}

template<typename T>
inline TLocalCoordinateSystem3<T>::TLocalCoordinateSystem3(
    const TVector3<T>& xAxis,
    const TVector3<T>& yAxis,
    const TVector3<T>& zAxis) :
    
    _xAxis(xAxis),
    _yAxis(yAxis),
    _zAxis(zAxis) {
}

template<typename T>
inline void TLocalCoordinateSystem3<T>::initializeViaUnitY(const TVector3<T>& unitY) {
    CADISE_ASSERT_RANGE_INCLUSIVE(unitY.lengthSquared(), T(0.999), T(1.001));

    TVector3<T> unitX;
    TVector3<T> unitZ;
    {
        if (std::abs(unitY.x()) > std::abs(unitY.y())) {
            TVector3<T> zAxis(-unitY.z(), T(0), unitY.x());
            zAxis.divLocal(std::sqrt(unitY.x() * unitY.x() + unitY.z() * unitY.z()));

            unitZ = zAxis;
        }
        else {
            TVector3<T> zAxis(T(0), unitY.z(), -unitY.y());
            zAxis.divLocal(std::sqrt(unitY.y() * unitY.y() + unitY.z() * unitY.z()));

            unitZ = zAxis;
        }

        unitX = unitY.cross(unitZ);
    }

    this->setAxes(unitX, unitY, unitZ);
}

template<typename T>
inline TVector3<T> TLocalCoordinateSystem3<T>::worldToLocal(const TVector3<T>& unitVector) const {
    return TVector3<T>(
        unitVector.dot(_xAxis),  // x-axis projection
        unitVector.dot(_yAxis),  // y-axis projection
        unitVector.dot(_zAxis)); // z-axis projection
}

template<typename T>
inline TVector3<T> TLocalCoordinateSystem3<T>::localToWorld(const TVector3<T>& unitVector) const {
    return _xAxis.mul(unitVector.x()).add(
           _yAxis.mul(unitVector.y())).add(
           _zAxis.mul(unitVector.z()));
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::cosTheta(const TVector3<T>& unitVector) const {
    return math::clamp(unitVector.dot(_yAxis), T(-1), T(1));
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::cos2Theta(const TVector3<T>& unitVector) const {
    return math::squared(this->cosTheta(unitVector));
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::sinTheta(const TVector3<T>& unitVector) const {
    return std::sqrt(this->sin2Theta(unitVector));
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::sin2Theta(const TVector3<T>& unitVector) const {
    return T(1) - this->cos2Theta(unitVector);
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::tanTheta(const TVector3<T>& unitVector) const {
    const T cosTheta = this->cosTheta(unitVector);
    const T sinTheta = this->sinTheta(unitVector);

    if (cosTheta != T(0)) {
        return math::clamp(
            sinTheta / cosTheta,
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::min());
    }
    else {
        // check sign of sinTheta to decide tanTheta is positive or negative
        switch (math::sign(sinTheta)) {
            case constant::SIGN_POSITIVE:
                return std::numeric_limits<T>::max();

            case constant::SIGN_NEGATIVE:
                return std::numeric_limits<T>::min();
        }

        CADISE_ASSERT(false);

        return T(0);
    }
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::tan2Theta(const TVector3<T>& unitVector) const {
    const T cos2Theta = this->cos2Theta(unitVector);
    const T sin2Theta = T(1) - cos2Theta;

    if (cos2Theta != T(0)) {
        return math::clamp(
            sin2Theta / cos2Theta,
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max());
    }
    else {
        return std::numeric_limits<T>::max();
    }
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::cosPhi(const TVector3<T>& unitVector) const {
    const T sin2Theta = this->sin2Theta(unitVector);
    
    if (sin2Theta != T(0)) {
        return unitVector.dot(_zAxis) / std::sqrt(sin2Theta);
    }
    else {
        return T(1); // for 0-degree phi
    }
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::cos2Phi(const TVector3<T>& unitVector) const {
    return math::squared(this->cosPhi(unitVector));
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::sinPhi(const TVector3<T>& unitVector) const {
    const T sin2Theta = this->sin2Theta(unitVector);

    if (sin2Theta != T(0)) {
        return unitVector.dot(_xAxis) / std::sqrt(sin2Theta);
    }
    else {
        return T(0); // for 0-degree phi
    }
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::sin2Phi(const TVector3<T>& unitVector) const {
    return math::squared(this->sinPhi(unitVector));
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::tanPhi(const TVector3<T>& unitVector) const {
    const T zProjection = unitVector.dot(_zAxis);
    const T xProjection = unitVector.dot(_xAxis);

    if (zProjection != T(0)) {
        return math::clamp(
            xProjection / zProjection,
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max());
    }
    else {
        // check sign of xProjection to decide tanPhi is positive or negative
        switch (math::sign(xProjection)) {
            case constant::SIGN_POSITIVE:
                return std::numeric_limits<T>::max();

            case constant::SIGN_NEGATIVE:
                return std::numeric_limits<T>::min();
        }

        CADISE_ASSERT(false);

        return T(0);
    }
}

template<typename T>
inline T TLocalCoordinateSystem3<T>::tan2Phi(const TVector3<T>& unitVector) const {
    const T zProjection2 = math::squared(unitVector.dot(_zAxis));
    const T xProjection2 = math::squared(unitVector.dot(_xAxis));

    if (zProjection2 != T(0)) {
        return math::clamp(
            xProjection2 / zProjection2,
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max());
    }
    else {
        return std::numeric_limits<T>::max();
    }
}

template<typename T>
inline const TVector3<T>& TLocalCoordinateSystem3<T>::xAxis() const {
    return _xAxis;
}

template<typename T>
inline const TVector3<T>& TLocalCoordinateSystem3<T>::yAxis() const {
    return _yAxis;
}

template<typename T>
inline const TVector3<T>& TLocalCoordinateSystem3<T>::zAxis() const {
    return _zAxis;
}

template<typename T>
inline void TLocalCoordinateSystem3<T>::setAxes(
    const TVector3<T>& xAxis,
    const TVector3<T>& yAxis,
    const TVector3<T>& zAxis) {

    this->setXAxis(xAxis);
    this->setYAxis(yAxis);
    this->setZAxis(zAxis);
}

template<typename T>
inline void TLocalCoordinateSystem3<T>::setXAxis(const TVector3<T>& xAxis) {
    _xAxis = xAxis;
}

template<typename T>
inline void TLocalCoordinateSystem3<T>::setYAxis(const TVector3<T>& yAxis) {
    _yAxis = yAxis;
}

template<typename T>
inline void TLocalCoordinateSystem3<T>::setZAxis(const TVector3<T>& zAxis) {
    _zAxis = zAxis;
}

} // namespace cadise