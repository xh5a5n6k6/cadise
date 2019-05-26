#pragma once

#include "math/vector.h"

#include <cassert>

namespace cadise {

template<typename T, uint64 Size>
inline Vector<T, Size>::Vector() {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) = static_cast<T>(0);
    }
}

template<typename T, uint64 Size>
inline Vector<T, Size>::Vector(T v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) = v;
    }
}

template<typename T, uint64 Size>
template<typename... Ts>
inline Vector<T, Size>::Vector(T v1, T v2, Ts... ts) :
    _v({ v1, v2, ts... }) {
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::operator-() {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = -_v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
template<typename U>
inline Vector<T, Size> Vector<T, Size>::operator+(const U s) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) + static_cast<T>(s);
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::operator+(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) + v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
template<typename U>
inline Vector<T, Size> Vector<T, Size>::operator-(const U s) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) - static_cast<T>(s);
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::operator-(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) - v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
template<typename U>
inline Vector<T, Size> Vector<T, Size>::operator*(const U s) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) * static_cast<T>(s);
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::operator*(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) * v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
template<typename U>
inline Vector<T, Size> Vector<T, Size>::operator/(const U s) {
    assert(static_cast<T>(s) != static_cast<T>(0));

    T invS = static_cast<T>(1) / static_cast<T>(s);
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) * invS;
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::operator/(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        assert(v._v.at(i) != static_cast<T>(0));

        result._v.at(i) = _v.at(i) / v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size>& Vector<T, Size>::operator+=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) += v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
inline Vector<T, Size>& Vector<T, Size>::operator-=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) -= v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
template<typename U>
inline Vector<T, Size>& Vector<T, Size>::operator*=(const U s) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) *= static_cast<T>(s);
    }

    return *this;
}

template<typename T, uint64 Size>
inline Vector<T, Size>& Vector<T, Size>::operator*=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) *= v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
template<typename U>
inline Vector<T, Size>& Vector<T, Size>::operator/=(const U s) {
    assert(static_cast<T>(s) != static_cast<T>(0));

    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) /= static_cast<T>(s);
    }

    return *this;
}

template<typename T, uint64 Size>
inline Vector<T, Size>& Vector<T, Size>::operator/=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        assert(v._v.at(i) != static_cast<T>(0));

        _v.at(i) /= v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
inline Vector<T, Size>& Vector<T, Size>::operator=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) = v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
inline bool Vector<T, Size>::isZero() {
    bool result = true;
    for (uint64 i = 0; i < Size; i++) {
        result &= _v.at(i) == static_cast<T>(0);
    }

    return result;
}

template<typename T, uint64 Size>
inline T Vector<T, Size>::length() {
    return static_cast<T>(std::sqrt(lengthSquared()));
}

template<typename T, uint64 Size>
inline T Vector<T, Size>::lengthSquared() {
    T result = static_cast<T>(0);
    for (uint64 i = 0; i < Size; i++) {
        result += _v.at(i) * _v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::normalize() {
    assert(length() > static_cast<T>(0));

    T invLength = static_cast<T>(1) / length();
    return *this * invLength;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::reciprocal() {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        assert(_v.at(i) != static_cast<T>(0));

        result._v.at(i) = static_cast<T>(1) / _v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::clamp(T min, T max) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = std::clamp(_v.at(i), min, max);
    }

    return result;
}

template<typename T, uint64 Size>
inline void Vector<T, Size>::swap(Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        std::swap(_v.at(i), v._v.at(i));
    }
}

template<typename T, uint64 Size>
inline T Vector<T, Size>::dot(Vector<T, Size> v) {
    T result = static_cast<T>(0);
    for (uint64 i = 0; i < Size; i++) {
        result += _v.at(i) * v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
inline T Vector<T, Size>::absDot(Vector<T, Size> v) {
    return static_cast<T>(std::abs(dot(v)));
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::cross(Vector<T, Size> v) {
    static_assert(Size == 3, "Error in vector's cross, this vector doesn't support cross method\n");

    return Vector<T, Size>(_v.at(1) * v._v.at(2) - _v.at(2) * v._v.at(1),
        _v.at(2) * v._v.at(0) - _v.at(0) * v._v.at(2),
        _v.at(0) * v._v.at(1) - _v.at(1) * v._v.at(0));
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::reflect(Vector<T, Size> normal) {
    Vector<T, Size> result = static_cast<T>(2) * absDot(normal) * normal;
    return result - *this;
}

template<typename T, uint64 Size>
inline T Vector<T, Size>::x() {
    return _v.at(0);
}

template<typename T, uint64 Size>
inline T Vector<T, Size>::y() {
    return _v.at(1);
}

template<typename T, uint64 Size>
inline T Vector<T, Size>::z() {
    static_assert(Size > 2, "Error in vector's z, this vector doesn't support z value\n");

    return _v.at(2);
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::min(Vector<T, Size> v1, Vector<T, Size> v2) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = std::min(v1._v.at(i), v2._v.at(i));
    }

    return result;
}

template<typename T, uint64 Size>
inline Vector<T, Size> Vector<T, Size>::max(Vector<T, Size> v1, Vector<T, Size> v2) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = std::max(v1._v.at(i), v2._v.at(i));
    }

    return result;
}

} // namespace cadise