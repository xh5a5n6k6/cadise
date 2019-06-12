#pragma once

#include "math/vector.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace cadise {

template<typename T, uint32 Size>
inline Vector<T, Size>::Vector() {
    for (uint32 i = 0; i < Size; i++) {
        _v[i] = static_cast<T>(0);
    }
}

template<typename T, uint32 Size>
inline Vector<T, Size>::Vector(const T v) {
    for (uint32 i = 0; i < Size; i++) {
        _v[i] = v;
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
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = -_v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator+(const T s) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = _v[i] + s;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator+(const Vector<T, Size> v) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = _v[i] + v._v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator-(const T s) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = _v[i] - s;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator-(const Vector<T, Size> v) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = _v[i] - v._v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator*(const T s) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = _v[i] * s;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator*(const Vector<T, Size> v) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = _v[i] * v._v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator/(const T s) const {
    assert(s != static_cast<T>(0));

    T invS = static_cast<T>(1) / s;
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = _v[i] * invS;
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::operator/(const Vector<T, Size> v) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        assert(v._v[i] != static_cast<T>(0));

        result._v[i] = _v[i] / v._v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator+=(const Vector<T, Size> v) {
    for (uint32 i = 0; i < Size; i++) {
        _v[i] += v._v[i];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator-=(const Vector<T, Size> v) {
    for (uint32 i = 0; i < Size; i++) {
        _v[i] -= v._v[i];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator*=(const T s) {
    for (uint32 i = 0; i < Size; i++) {
        _v[i] *= s;
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator*=(const Vector<T, Size> v) {
    for (uint32 i = 0; i < Size; i++) {
        _v[i] *= v._v[i];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator/=(const T s) {
    assert(s != static_cast<T>(0));

    for (uint32 i = 0; i < Size; i++) {
        _v[i] /= s;
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator/=(const Vector<T, Size> v) {
    for (uint32 i = 0; i < Size; i++) {
        assert(v._v[i] != static_cast<T>(0));

        _v[i] /= v._v[i];
    }

    return *this;
}

template<typename T, uint32 Size>
inline Vector<T, Size>& Vector<T, Size>::operator=(const Vector<T, Size> v) {
    for (uint32 i = 0; i < Size; i++) {
        _v[i] = v._v[i];
    }

    return *this;
}

template<typename T, uint32 Size>
inline bool Vector<T, Size>::isZero() const {
    bool result = true;
    for (uint32 i = 0; i < Size; i++) {
        result &= _v[i] == static_cast<T>(0);
    }

    return result;
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::length() const {
    return static_cast<T>(std::sqrt(lengthSquared()));
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::lengthSquared() const {
    T result = static_cast<T>(0);
    for (uint32 i = 0; i < Size; i++) {
        result += _v[i] * _v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::normalize() const {
    assert(length() > static_cast<T>(0));

    T invLength = static_cast<T>(1) / length();
    return *this * invLength;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::reciprocal() const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        assert(_v[i] != static_cast<T>(0));

        result._v[i] = static_cast<T>(1) / _v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::clamp(const T min, const T max) const {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = std::clamp(_v[i], min, max);
    }

    return result;
}

template<typename T, uint32 Size>
inline void Vector<T, Size>::swap(Vector<T, Size> &v) {
    for (uint32 i = 0; i < Size; i++) {
        std::swap(_v[i], v._v[i]);
    }
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::dot(const Vector<T, Size> v) const {
    T result = static_cast<T>(0);
    for (uint32 i = 0; i < Size; i++) {
        result += _v[i] * v._v[i];
    }

    return result;
}

template<typename T, uint32 Size>
inline T Vector<T, Size>::absDot(const Vector<T, Size> v) const {
    return std::abs(dot(v));
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::cross(const Vector<T, Size> v) const {
    static_assert(Size == 3, "Error in vector's cross, this vector doesn't support cross method\n");

    return Vector<T, Size>(_v[1] * v._v[2] - _v[2] * v._v[1],
                           _v[2] * v._v[0] - _v[0] * v._v[2],
                           _v[0] * v._v[1] - _v[1] * v._v[0]);
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::reflect(const Vector<T, Size> normal) const {
    Vector<T, Size> result = static_cast<T>(2) * absDot(normal) * normal;
    return result - *this;
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
inline Vector<T, Size> Vector<T, Size>::min(const Vector<T, Size> v1, const Vector<T, Size> v2) {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = std::min(v1._v[i], v2._v[i]);
    }

    return result;
}

template<typename T, uint32 Size>
inline Vector<T, Size> Vector<T, Size>::max(const Vector<T, Size> v1, const Vector<T, Size> v2) {
    Vector<T, Size> result;
    for (uint32 i = 0; i < Size; i++) {
        result._v[i] = std::max(v1._v[i], v2._v[i]);
    }

    return result;
}

} // namespace cadise