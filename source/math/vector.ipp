#pragma once

#include "math/vector.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <cmath>

namespace cadise {

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::min(const Vector<T, N>& v1, const Vector<T, N>& v2) {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = std::min(v1._v[i], v2._v[i]);
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::max(const Vector<T, N>& v1, const Vector<T, N>& v2) {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = std::max(v1._v[i], v2._v[i]);
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N>::Vector() :
    Vector(static_cast<T>(0)) {
}

template<typename T, std::size_t N>
inline Vector<T, N>::Vector(const T v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] = v;
    }
}

template<typename T, std::size_t N>
template<typename... Ts>
inline Vector<T, N>::Vector(const T v1, const T v2, const Ts... ts) :
    _v({ v1, v2, ts... }) {
}

template<typename T, std::size_t N>
template<typename U>
inline Vector<T, N>::Vector(const Vector<U, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] = static_cast<T>(v[i]);
    }
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator-() const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = -_v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator+(const T s) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] + s;
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator-(const T s) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] - s;
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator*(const T s) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * s;
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator/(const T s) const {
    const T inverseS = static_cast<T>(1) / s;
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * inverseS;
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator+(const Vector<T, N>& v) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] + v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator-(const Vector<T, N>& v) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] - v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator*(const Vector<T, N>& v) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::operator/(const Vector<T, N>& v) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] / v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator+=(const T s) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] += s;
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator-=(const T s) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] -= s;
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator*=(const T s) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] *= s;
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator/=(const T s) {
    const T inverseS = static_cast<T>(1) / s;
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] *= inverseS;
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator+=(const Vector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] += v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator-=(const Vector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] -= v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator*=(const Vector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] *= v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator/=(const Vector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] /= v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline Vector<T, N>& Vector<T, N>::operator=(const Vector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] = v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline T& Vector<T, N>::operator[](const std::size_t i) {
    return _v[i];
}

template<typename T, std::size_t N>
inline const T& Vector<T, N>::operator[](const std::size_t i) const {
    return _v[i];
}

template<typename T, std::size_t N>
inline bool Vector<T, N>::isZero() const {
    bool result = true;
    for (std::size_t i = 0; i < N; ++i) {
        result &= _v[i] == static_cast<T>(0);
    }

    return result;
}

template<typename T, std::size_t N>
inline bool Vector<T, N>::hasNaN() const {
    for (std::size_t i = 0; i < N; ++i) {
        if (std::isnan(_v[i])) {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline bool Vector<T, N>::hasInfinite() const {
    for (std::size_t i = 0; i < N; ++i) {
        if (!std::isfinite(_v[i])) {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline T Vector<T, N>::sum() const {
    T result = static_cast<T>(0);
    for (std::size_t i = 0; i < N; ++i) {
        result += _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T Vector<T, N>::average() const {
    return sum() / static_cast<T>(N);
}

template<typename T, std::size_t N>
inline T Vector<T, N>::length() const {
    return static_cast<T>(std::sqrt(lengthSquared()));
}

template<typename T, std::size_t N>
inline T Vector<T, N>::lengthSquared() const {
    T result = static_cast<T>(0);
    for (std::size_t i = 0; i < N; ++i) {
        result += _v[i] * _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T Vector<T, N>::maxComponent() const {
    return _v[maxDimension()];
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::normalize() const {
    CADISE_ASSERT(length() > static_cast<T>(0));

    const T invLength = static_cast<T>(1) / length();
    return *this * invLength;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::reverse() const {
    return -*this;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::square() const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::complement() const {
    const Vector<T, N> oneVector(static_cast<T>(1));
    return oneVector - *this;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::reciprocal() const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = static_cast<T>(1) / _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::clamp(const T min, const T max) const {
    Vector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = math::clamp(_v[i], min, max);
    }

    return result;
}

template<typename T, std::size_t N>
inline std::size_t Vector<T, N>::maxDimension() const {
    T max = _v[0];
    std::size_t maxD = 0;
    for (std::size_t i = 1; i < N; ++i) {
        if (_v[i] > max) {
            max = _v[i];
            maxD = i;
        }
    }

    return maxD;
}

template<typename T, std::size_t N>
inline void Vector<T, N>::swap(Vector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        math::swap(_v[i], v._v[i]);
    }
}

template<typename T, std::size_t N>
inline bool Vector<T, N>::equals(const Vector<T, N>& rhs) const {
    for (std::size_t i = 0; i < N; ++i) {
        if (_v[i] != rhs._v[i]) {
            return false;
        }
    }

    return true;
}

template<typename T, std::size_t N>
inline T Vector<T, N>::dot(const Vector<T, N>& v) const {
    T result = static_cast<T>(0);
    for (std::size_t i = 0; i < N; ++i) {
        result += _v[i] * v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T Vector<T, N>::absDot(const Vector<T, N>& v) const {
    return std::abs(dot(v));
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::lerp(const Vector<T, N>& rhs, const real ratio) const {
    Vector<T, N> result;
    const real ratioComplement = 1.0_r - ratio;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = ratioComplement * _v[i] + ratio * rhs._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::cross(const Vector<T, N>& v) const {
    static_assert(N == 3, "Error in vector's cross, this vector doesn't support cross method\n");

    return Vector<T, N>(_v[1] * v._v[2] - _v[2] * v._v[1],
                        _v[2] * v._v[0] - _v[0] * v._v[2],
                        _v[0] * v._v[1] - _v[1] * v._v[0]);
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::reflect(const Vector<T, N>& normal) const {
    const Vector<T, N> result = static_cast<T>(2) * absDot(normal) * normal;
    return result - *this;
}

template<typename T, std::size_t N>
inline Vector<T, N> Vector<T, N>::refract(const Vector<T, N>& normal, const real iorOuter, const real iorInner) const {
    real etaI = iorOuter;
    real etaT = iorInner;
    real signFactor = 1.0_r;

    const real cosI = dot(normal);

    // check if incident ray is from inner to outer
    if (cosI < 0.0_r) {
        math::swap(etaI, etaT);
        signFactor = -1.0_r;
    }

    const real etaRatio = etaI / etaT;
    const real sinT2 = etaRatio * etaRatio * (1.0_r - cosI * cosI);

    // handle TIR condition
    if (sinT2 > 1.0_r) {
        return Vector<T, N>(static_cast<T>(0));
    }

    const real cosT = std::sqrt(1.0_r - sinT2);
    Vector<T, N> refractDirection = -etaRatio * *this;
    refractDirection += (etaRatio * signFactor * cosI - cosT) * signFactor * normal;
    refractDirection = refractDirection.normalize();

    return refractDirection;
}

template<typename T, std::size_t N>
inline T Vector<T, N>::x() const {
    return _v[0];
}

template<typename T, std::size_t N>
inline T Vector<T, N>::y() const {
    return _v[1];
}

template<typename T, std::size_t N>
inline T Vector<T, N>::z() const {
    static_assert(N > 2, "Error in vector's z, this vector doesn't support z value\n");

    return _v[2];
}

} // namespace cadise