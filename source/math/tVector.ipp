#pragma once

#include "math/tVector.h"

#include "fundamental/assertion.h"
#include "math/math.h"

#include <cmath>
#include <type_traits>

namespace cadise {

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::min(const TVector<T, N>& vA, const TVector<T, N>& vB) {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = std::min(vA._v[i], vB._v[i]);
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::max(const TVector<T, N>& vA, const TVector<T, N>& vB) {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = std::max(vA._v[i], vB._v[i]);
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N>::TVector() :
    TVector(static_cast<T>(0)) {
}

template<typename T, std::size_t N>
inline TVector<T, N>::TVector(const T v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] = v;
    }
}

template<typename T, std::size_t N>
template<typename... Ts>
inline TVector<T, N>::TVector(const T v1, const T v2, const Ts... ts) :
    _v({ v1, v2, ts... }) {
}

template<typename T, std::size_t N>
inline TVector<T, N>::TVector(const TVector<T, N>& other) = default;

template<typename T, std::size_t N>
template<typename U>
inline TVector<T, N>::TVector(const TVector<U, N>& other) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] = static_cast<T>(other[i]);
    }
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator - () const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = -_v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator + (const T s) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] + s;
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator - (const T s) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] - s;
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator * (const T s) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * s;
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator / (const T s) const {
    const T inverseS = static_cast<T>(1) / s;
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * inverseS;
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator + (const TVector<T, N>& v) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] + v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator - (const TVector<T, N>& v) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] - v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator * (const TVector<T, N>& v) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::operator / (const TVector<T, N>& v) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] / v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator += (const T s) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] += s;
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator -= (const T s) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] -= s;
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator *= (const T s) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] *= s;
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator /= (const T s) {
    const T inverseS = static_cast<T>(1) / s;
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] *= inverseS;
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator += (const TVector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] += v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator -= (const TVector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] -= v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator *= (const TVector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] *= v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator /= (const TVector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] /= v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline TVector<T, N>& TVector<T, N>::operator = (const TVector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        _v[i] = v._v[i];
    }

    return *this;
}

template<typename T, std::size_t N>
inline T& TVector<T, N>::operator [] (const std::size_t i) {
    CADISE_ASSERT_LT(i, N);

    return _v[i];
}

template<typename T, std::size_t N>
inline const T& TVector<T, N>::operator [] (const std::size_t i) const {
    CADISE_ASSERT_LT(i, N);

    return _v[i];
}

template<typename T, std::size_t N>
template<typename U>
inline TVector<U, N> TVector<T, N>::asType() const {
    return TVector<U, N>(*this);
}

template<typename T, std::size_t N>
inline bool TVector<T, N>::isZero() const {
    bool result = true;
    for (std::size_t i = 0; i < N; ++i) {
        result &= _v[i] == static_cast<T>(0);
    }

    return result;
}

template<typename T, std::size_t N>
inline bool TVector<T, N>::hasNaN() const {
    for (std::size_t i = 0; i < N; ++i) {
        if (std::isnan(_v[i])) {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline bool TVector<T, N>::hasNegative() const {
    static_assert(std::is_signed_v<T>);

    for (std::size_t i = 0; i < N; ++i) {
        if (_v[i] < static_cast<T>(0)) {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline bool TVector<T, N>::hasInfinite() const {
    for (std::size_t i = 0; i < N; ++i) {
        if (!std::isfinite(_v[i])) {
            return true;
        }
    }

    return false;
}

template<typename T, std::size_t N>
inline T TVector<T, N>::sum() const {
    T result = static_cast<T>(0);
    for (std::size_t i = 0; i < N; ++i) {
        result += _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T TVector<T, N>::average() const {
    return this->sum() / static_cast<T>(N);
}

template<typename T, std::size_t N>
inline T TVector<T, N>::length() const {
    return static_cast<T>(std::sqrt(this->lengthSquared()));
}

template<typename T, std::size_t N>
inline T TVector<T, N>::lengthSquared() const {
    T result = static_cast<T>(0);
    for (std::size_t i = 0; i < N; ++i) {
        result += _v[i] * _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T TVector<T, N>::product() const {
    T result = static_cast<T>(1);
    for (std::size_t i = 0; i < N; ++i) {
        result *= _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T TVector<T, N>::maxComponent() const {
    return _v[this->maxDimension()];
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::normalize() const {
    CADISE_ASSERT_GT(this->length(), static_cast<T>(0));

    const T invLength = static_cast<T>(1) / this->length();
    return *this * invLength;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::reverse() const {
    return -*this;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::square() const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = _v[i] * _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::complement() const {
    const TVector<T, N> oneTVector(static_cast<T>(1));
    return oneTVector - *this;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::reciprocal() const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = static_cast<T>(1) / _v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::clamp(const T min, const T max) const {
    TVector<T, N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = math::clamp(_v[i], min, max);
    }

    return result;
}

template<typename T, std::size_t N>
inline std::size_t TVector<T, N>::maxDimension() const {
    T           max  = _v[0];
    std::size_t maxD = 0;
    for (std::size_t i = 1; i < N; ++i) {
        if (_v[i] > max) {
            max  = _v[i];
            maxD = i;
        }
    }

    return maxD;
}

template<typename T, std::size_t N>
inline void TVector<T, N>::swap(TVector<T, N>& v) {
    for (std::size_t i = 0; i < N; ++i) {
        math::swap(_v[i], v._v[i]);
    }
}

template<typename T, std::size_t N>
inline bool TVector<T, N>::isEqualTo(const TVector<T, N>& rhs) const {
    for (std::size_t i = 0; i < N; ++i) {
        if (_v[i] != rhs._v[i]) {
            return false;
        }
    }

    return true;
}

template<typename T, std::size_t N>
inline T TVector<T, N>::dot(const TVector<T, N>& v) const {
    T result = static_cast<T>(0);
    for (std::size_t i = 0; i < N; ++i) {
        result += _v[i] * v._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline T TVector<T, N>::absDot(const TVector<T, N>& v) const {
    return std::abs(this->dot(v));
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::lerp(const TVector<T, N>& rhs, const real ratio) const {
    TVector<T, N> result;
    const real ratioComplement = 1.0_r - ratio;
    for (std::size_t i = 0; i < N; ++i) {
        result._v[i] = ratioComplement * _v[i] + ratio * rhs._v[i];
    }

    return result;
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::cross(const TVector<T, N>& v) const {
    static_assert(N == 3, "Error in vector's cross, this vector doesn't support cross method\n");

    return TVector<T, N>(
        _v[1] * v._v[2] - _v[2] * v._v[1],
        _v[2] * v._v[0] - _v[0] * v._v[2],
        _v[0] * v._v[1] - _v[1] * v._v[0]);
}

template<typename T, std::size_t N>
inline TVector<T, N> TVector<T, N>::reflect(const TVector<T, N>& normal) const {
    const TVector<T, N> result = static_cast<T>(2) * this->absDot(normal) * normal;
    return result - *this;
}

template<typename T, std::size_t N>
inline bool TVector<T, N>::canRefract(
    const TVector<T, N>& normal,
    const real           iorOuter,
    const real           iorInner,
    TVector<T, N>* const out_vector) const {

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
    TVector<T, N> refractDirection = -etaRatio * *this;
    refractDirection += (etaRatio * signFactor * cosI - cosT) * signFactor * normal;
    refractDirection  = refractDirection.normalize();

    *out_vector = refractDirection;

    return true;
}

template<typename T, std::size_t N>
inline T TVector<T, N>::x() const {
    return _v[0];
}

template<typename T, std::size_t N>
inline T TVector<T, N>::y() const {
    return _v[1];
}

template<typename T, std::size_t N>
inline T TVector<T, N>::z() const {
    static_assert(N > 2, "Error in vector's z, this vector doesn't support z value\n");

    return _v[2];
}

} // namespace cadise