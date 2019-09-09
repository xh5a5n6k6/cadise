#pragma once

#include "math/vector.h"

#include "fundamental/assertion.h"

#include "math/math.h"

#include <cmath>

namespace cadise {

template<typename T, uint32 Size>
inline Vector<T, Size>::Vector() {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] = static_cast<T>(0);
    }
}

template<typename T, uint32 Size>
inline Vector<T, Size>::Vector(const T v) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] = v;
    }
}

template<typename T, uint32 Size>
template<typename... Ts>
inline Vector<T, Size>::Vector(const T v1, const T v2, const Ts... ts) :
    _v({ v1, v2, ts... }) {
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator-() const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = -_v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator+(const T s) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] + s;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator-(const T s) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] - s;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator*(const T s) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] * s;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator/(const T s) const {
    T invS = static_cast<T>(1) / s;
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] * invS;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator+(const Vector<T, Size>& v) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] + v._v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator-(const Vector<T, Size>& v) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] - v._v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator*(const Vector<T, Size>& v) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] * v._v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator/(const Vector<T, Size>& v) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = _v[index] / v._v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator+=(const T s) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] += s;
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator-=(const T s) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] -= s;
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator*=(const T s) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] *= s;
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator/=(const T s) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] /= s;
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator+=(const Vector<T, Size>& v) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] += v._v[index];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator-=(const Vector<T, Size>& v) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] -= v._v[index];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator*=(const Vector<T, Size>& v) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] *= v._v[index];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator/=(const Vector<T, Size>& v) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] /= v._v[index];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator=(const Vector<T, Size>& v) {
    for (uint32 index = 0; index < Size; index++) {
        _v[index] = v._v[index];
    }

    return *this;
}

template<typename T, uint32 Size>
inline T& Vector<T, Size>::operator[](const uint32 index) {
    return _v[index];
}

template<typename T, uint32 Size>
inline bool Vector<T, Size>::isZero() const {
    bool result = true;
    for (uint32 index = 0; index < Size; index++) {
        result &= _v[index] == static_cast<T>(0);
    }

    return result;
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::sum() const {
    T result = static_cast<T>(0);
    for (uint32 index = 0; index < Size; index++) {
        result += _v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::average() const {
    return sum() / static_cast<T>(Size);
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::length() const {
    return static_cast<T>(std::sqrt(lengthSquared()));
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::lengthSquared() const {
    T result = static_cast<T>(0);
    for (uint32 index = 0; index < Size; index++) {
        result += _v[index] * _v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::maxComponent() const {
    return _v[maxDimension()];
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::normalize() const {
    CADISE_ASSERT(length() > static_cast<T>(0));

    T invLength = static_cast<T>(1) / length();
    return *this * invLength;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::composite() const {
    return -*this;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::complement() const {
    Vector<T, Size> oneVector(static_cast<T>(1));
    return oneVector - *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::reciprocal() const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = static_cast<T>(1) / _v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::clamp(const T min, const T max) const {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = math::clamp(_v[index], min, max);
    }

    return result;
}

template<typename T, uint32 Size>
inline uint32 Vector<T, Size>::maxDimension() const {
    T max = _v[0];
    uint32 maxD = 0;
    for (uint32 index = 1; index < Size; index++) {
        if (_v[index] > max) {
            max = _v[index];
            maxD = index;
        }
    }

    return maxD;
}

template<typename T, uint32 Size>
inline void Vector<T, Size>::swap(Vector<T, Size>& v) {
    for (uint32 index = 0; index < Size; index++) {
        math::swap(_v[index], v._v[index]);
    }
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::dot(const Vector<T, Size>& v) const {
    T result = static_cast<T>(0);
    for (uint32 index = 0; index < Size; index++) {
        result += _v[index] * v._v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::absDot(const Vector<T, Size>& v) const {
    return std::abs(dot(v));
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::lerp(const Vector<T, Size>& rhs, const real ratio) const {
    Vector<T, Size> result;
    real ratioComplement = 1.0_r - ratio;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = ratioComplement * _v[index] + ratio * rhs._v[index];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::cross(const Vector<T, Size>& v) const {
    static_assert(Size == 3, "Error in vector's cross, this vector doesn't support cross method\n");

    return Vector<T, Size>(_v[1] * v._v[2] - _v[2] * v._v[1],
                           _v[2] * v._v[0] - _v[0] * v._v[2],
                           _v[0] * v._v[1] - _v[1] * v._v[0]);
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::reflect(const Vector<T, Size>& normal) const {
    Vector<T, Size> result = static_cast<T>(2) * absDot(normal) * normal;
    return result - *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::refract(const Vector<T, Size>& normal, const real iorOuter, const real iorInner) const {
    real cosI = dot(normal);
    real etaI = iorOuter;
    real etaT = iorInner;
    real signFactor = 1.0_r;

    // check if incident ray is from inner to outer
    if (cosI < 0.0_r) {
        math::swap(etaI, etaT);
        signFactor = -1.0_r;
    }

    real etaRatio = etaI / etaT;
    real sin2_T = etaRatio * etaRatio * (1.0_r - cosI * cosI);

    // handle TIR condition
    if (sin2_T > 1.0_r) {
        return Vector<T, Size>(static_cast<T>(0));
    }

    real cosT = std::sqrt(1.0_r - sin2_T);
    Vector<T, Size> refractDirection = -etaRatio * *this;
    refractDirection += (etaRatio * signFactor * cosI - cosT) * signFactor * normal;
    refractDirection = refractDirection.normalize();

    return refractDirection;
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::x() const {
    return _v[0];
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::y() const {
    return _v[1];
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::z() const {
    static_assert(Size > 2, "Error in vector's z, this vector doesn't support z value\n");

    return _v[2];
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::min(const Vector<T, Size>& v1, const Vector<T, Size>& v2) {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = std::min(v1._v[index], v2._v[index]);
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::max(const Vector<T, Size>& v1, const Vector<T, Size>& v2) {
    Vector<T, Size> result;
    for (uint32 index = 0; index < Size; index++) {
        result._v[index] = std::max(v1._v[index], v2._v[index]);
    }

    return result;
}

} // namespace cadise