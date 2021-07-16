#pragma once

#include "math/tVector3.h"

#include "fundamental/assertion.h"
#include "math/tArithmeticArray.h"
#include "math/math.h"

#include <cmath>

namespace cadise {

template<typename T>
inline TVector3<T> TVector3<T>::min(const TVector3<T>& vectorA, const TVector3<T>& vectorB) {
    return TVector3<T>(
        math::min(vectorA._x, vectorB._x),
        math::min(vectorA._y, vectorB._y),
        math::min(vectorA._z, vectorB._z));
}

template<typename T>
inline TVector3<T> TVector3<T>::max(const TVector3<T>& vectorA, const TVector3<T>& vectorB) {
    return TVector3<T>(
        math::max(vectorA._x, vectorB._x),
        math::max(vectorA._y, vectorB._y),
        math::max(vectorA._z, vectorB._z));
}

template<typename T>
inline TVector3<T>::TVector3() :
    TVector3(static_cast<T>(0)) {
}

template<typename T>
inline TVector3<T>::TVector3(const T value) :
    TVector3(value, value, value) {
}

template<typename T>
inline TVector3<T>::TVector3(const TArithmeticArray<T, 3>& value) :
    TVector3(value[0], value[1], value[2]) {
}

template<typename T>
inline TVector3<T>::TVector3(const T x, const T y, const T z) :
    _x(x),
    _y(y),
    _z(z) {
}

template<typename T>
inline TVector3<T>::TVector3(const TVector3<T>& other) = default;

template<typename T>
template<typename U>
inline TVector3<T>::TVector3(const TVector3<U>& other) :
    TVector3(
        static_cast<T>(other.x()),
        static_cast<T>(other.y()),
        static_cast<T>(other.z())) {
}

template<typename T>
inline TVector3<T> TVector3<T>::operator + (const T rhs) const {
    return this->add(rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator - (const T rhs) const {
    return this->sub(rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator * (const T rhs) const {
    return this->mul(rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator / (const T rhs) const {
    return this->div(rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator + (const TVector3<T>& rhs) const {
    return this->add(rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator - (const TVector3<T>& rhs) const {
    return this->sub(rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator * (const TVector3<T>& rhs) const {
    return this->mul(rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::operator / (const TVector3<T>& rhs) const {
    return this->div(rhs);
}

template<typename T>
inline T& TVector3<T>::operator [] (const constant::AxisType index) {
    CADISE_ASSERT_LT(index, 3);

    switch (index) {
        case constant::AXIS_X:
            return _x;
        
        case constant::AXIS_Y:
            return _y;
        
        case constant::AXIS_Z:
            return _z;
    }

    CADISE_ASSERT(false);

    return _x;
}

template<typename T>
inline const T& TVector3<T>::operator [] (const constant::AxisType index) const {
    CADISE_ASSERT_LT(index, 3);

    switch (index) {
        case constant::AXIS_X:
            return _x;

        case constant::AXIS_Y:
            return _y;

        case constant::AXIS_Z:
            return _z;
    }

    CADISE_ASSERT(false);

    return _x;
}

template<typename T>
template<typename U>
inline TVector3<U> TVector3<T>::asType() const {
    return TVector3<U>(*this);
}

template<typename T>
inline TVector3<T> TVector3<T>::add(const TVector3<T>& rhs) const {
    return this->add(rhs._x, rhs._y, rhs._z);
}

template<typename T>
inline TVector3<T> TVector3<T>::add(const T rhs) const {
    return this->add(rhs, rhs, rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::add(const T x, const T y, const T z) const {
    return TVector3<T>(_x + x, _y + y, _z + z);
}

template<typename T>
inline TVector3<T>& TVector3<T>::addLocal(const TVector3<T>& rhs) {
    _x += rhs._x;
    _y += rhs._y;
    _z += rhs._z;

    return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::addLocal(const T rhs) {
    _x += rhs;
    _y += rhs;
    _z += rhs;

    return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::sub(const TVector3<T>& rhs) const {
    return this->sub(rhs._x, rhs._y, rhs._z);
}

template<typename T>
inline TVector3<T> TVector3<T>::sub(const T rhs) const {
    return this->sub(rhs, rhs, rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::sub(const T x, const T y, const T z) const {
    return TVector3<T>(_x - x, _y - y, _z - z);
}

template<typename T>
inline TVector3<T>& TVector3<T>::subLocal(const TVector3<T>& rhs) {
    _x -= rhs._x;
    _y -= rhs._y;
    _z -= rhs._z;

    return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::subLocal(const T rhs) {
    _x -= rhs;
    _y -= rhs;
    _z -= rhs;

    return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::mul(const TVector3<T>& rhs) const {
    return this->mul(rhs._x, rhs._y, rhs._z);
}

template<typename T>
inline TVector3<T> TVector3<T>::mul(const T rhs) const {
    return this->mul(rhs, rhs, rhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::mul(const T x, const T y, const T z) const {
    return TVector3<T>(_x * x, _y * y, _z * z);
}

template<typename T>
inline TVector3<T>& TVector3<T>::mulLocal(const TVector3<T>& rhs) {
    _x *= rhs._x;
    _y *= rhs._y;
    _z *= rhs._z;

    return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::mulLocal(const T rhs) {
    _x *= rhs;
    _y *= rhs;
    _z *= rhs;

    return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::div(const TVector3<T>& rhs) const {
    return this->div(rhs._x, rhs._y, rhs._z);
}

template<typename T>
inline TVector3<T> TVector3<T>::div(const T rhs) const {
    const T rcpRhs = static_cast<T>(1) / rhs;

    return this->mul(rcpRhs, rcpRhs, rcpRhs);
}

template<typename T>
inline TVector3<T> TVector3<T>::div(const T x, const T y, const T z) const {
    return TVector3<T>(_x / x, _y / y, _z / z);
}

template<typename T>
inline TVector3<T>& TVector3<T>::divLocal(const TVector3<T>& rhs) {
    _x /= rhs._x;
    _y /= rhs._y;
    _z /= rhs._z;

    return *this;
}

template<typename T>
inline TVector3<T>& TVector3<T>::divLocal(const T rhs) {
    const T rcpRhs = static_cast<T>(1) / rhs;
    
    _x *= rcpRhs;
    _y *= rcpRhs;
    _z *= rcpRhs;

    return *this;
}

template<typename T>
inline bool TVector3<T>::isZero() const {
    return _x == static_cast<T>(0) &&
           _y == static_cast<T>(0) &&
           _z == static_cast<T>(0);
}

template<typename T>
inline bool TVector3<T>::hasNaN() const {
    return std::isnan(_x) || std::isnan(_y) || std::isnan(_z);
}

template<typename T>
inline bool TVector3<T>::hasNegative() const {
    return _x < static_cast<T>(0) ||
           _y < static_cast<T>(0) ||
           _z < static_cast<T>(0);
}

template<typename T>
inline bool TVector3<T>::hasInfinite() const {
    return !std::isfinite(_x) || !std::isfinite(_y) || !std::isfinite(_z);
}

template<typename T>
inline T TVector3<T>::sum() const {
    return _x + _y + _z;
}

template<typename T>
inline T TVector3<T>::average() const {
    return this->sum() / static_cast<T>(3);
}

template<typename T>
inline T TVector3<T>::length() const {
    return static_cast<T>(std::sqrt(this->lengthSquared()));
}

template<typename T>
inline T TVector3<T>::lengthSquared() const {
    return _x * _x + _y * _y + _z * _z;
}

template<typename T>
inline T TVector3<T>::maxComponent() const {
    return math::max(math::max(_x, _y), _z);
}

template<typename T>
inline constant::AxisType TVector3<T>::maxDimension() const {
    const int32 bits = ((_x < _y) << 2) + ((_x < _z) << 1) + (_y < _z);

    return constant::compareMaxTable[bits];
}

template<typename T>
inline TVector3<T> TVector3<T>::normalize() const {
    CADISE_ASSERT_GT(this->length(), static_cast<T>(0));

    const T rcpLength = static_cast<T>(1) / this->length();

    return TVector3(_x * rcpLength, _y * rcpLength, _z * rcpLength);
}

template<typename T>
inline TVector3<T>& TVector3<T>::normalizeLocal() {
    CADISE_ASSERT_GT(this->length(), static_cast<T>(0));

    const T rcpLength = static_cast<T>(1) / this->length();

    _x *= rcpLength;
    _y *= rcpLength;
    _z *= rcpLength;

    return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::negate() const {
    static_assert(std::is_signed_v<T>);

    return TVector3<T>(-_x, -_y, -_z);
}

template<typename T>
inline TVector3<T>& TVector3<T>::negateLocal() {
    static_assert(std::is_signed_v<T>);

    _x = -_x;
    _y = -_y;
    _z = -_z;

    return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::reciprocal() const {
    return TVector3<T>(
        static_cast<T>(1) / _x, 
        static_cast<T>(1) / _y, 
        static_cast<T>(1) / _z);
}

template<typename T>
inline TVector3<T>& TVector3<T>::reciprocalLocal() {
    _x = static_cast<T>(1) / _x;
    _y = static_cast<T>(1) / _y;
    _z = static_cast<T>(1) / _z;

    return *this;
}

template<typename T>
inline TVector3<T> TVector3<T>::clamp(const T min, const T max) const {
    return TVector3<T>(
        math::clamp(_x, min, max),
        math::clamp(_y, min, max),
        math::clamp(_z, min, max));
}

template<typename T>
inline TVector3<T>& TVector3<T>::clampLocal(const T min, const T max) {
    _x = math::clamp(_x, min, max);
    _y = math::clamp(_y, min, max);
    _z = math::clamp(_z, min, max);

    return *this;

}

template<typename T>
inline void TVector3<T>::swap(TVector3<T>& rhs) {
    math::swap(_x, rhs._x);
    math::swap(_y, rhs._y);
    math::swap(_z, rhs._z);
}

template<typename T>
inline bool TVector3<T>::isEqualTo(const TVector3<T>& rhs) const {
    return _x == rhs._x && _y == rhs._y && _z == rhs._z;
}

template<typename T>
inline T TVector3<T>::dot(const TVector3<T>& rhs) const {
    return _x * rhs._x + _y * rhs._y + _z * rhs._z;
}

template<typename T>
inline T TVector3<T>::absDot(const TVector3<T>& rhs) const {
    return std::abs(this->dot(rhs));
}

template<typename T>
inline TVector3<T> TVector3<T>::lerp(const TVector3<T>& rhs, const T ratio) const {
    const T ratioComplement = static_cast<T>(1) - ratio;

    TVector3<T> result(*this);

    return result.mul(ratioComplement).add(rhs.mul(ratio));
}

template<typename T>
inline TVector3<T> TVector3<T>::cross(const TVector3<T>& rhs) const {
    return TVector3<T>(
        _y * rhs._z - _z * rhs._y,
        _z * rhs._x - _x * rhs._z,
        _x * rhs._y - _y * rhs._x);
}

template<typename T>
inline TVector3<T> TVector3<T>::reflect(const TVector3<T>& normal) const {
    const TVector3<T> result = normal.mul(static_cast<T>(2) * this->absDot(normal));

    return result.sub(*this);
}

template<typename T>
inline bool TVector3<T>::canRefract(
    const TVector3<T>& normal,
    const real         iorOuter,
    const real         iorInner,
    TVector3<T>* const out_vector) const {

    CADISE_ASSERT(out_vector);

    real etaI       = iorOuter;
    real etaT       = iorInner;
    real signFactor = 1.0_r;

    // check if incident ray is from inner to outer
    const real cosI = this->dot(normal);
    if (cosI < 0.0_r) {
        math::swap(etaI, etaT);
        signFactor = -1.0_r;
    }

    const real etaRatio = etaI / etaT;
    const real sinT2    = etaRatio * etaRatio * (1.0_r - cosI * cosI);

    // handle TIR condition
    if (sinT2 >= 1.0_r) {
        return false;
    }

    const real cosT = std::sqrt(1.0_r - sinT2);
    TVector3<T> refractDirection = this->mul(-etaRatio);
    refractDirection.addLocal(normal.mul(signFactor * (etaRatio * signFactor * cosI - cosT)));
    refractDirection.normalizeLocal();

    out_vector->set(refractDirection);

    return true;
}

template<typename T>
inline T TVector3<T>::x() const {
    return _x;
}

template<typename T>
inline T TVector3<T>::y() const {
    return _y;
}

template<typename T>
inline T TVector3<T>::z() const {
    return _z;
}

template<typename T>
inline void TVector3<T>::set(const TVector3<T>& other) {
    this->set(other._x, other._y, other._z);
}

template<typename T>
inline void TVector3<T>::set(const T value) {
    this->set(value, value, value);
}

template<typename T>
inline void TVector3<T>::set(const T x, const T y, const T z) {
    _x = x;
    _y = y;
    _z = z;
}

} // namespace cadise