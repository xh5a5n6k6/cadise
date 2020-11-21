#pragma once

#include "math/type/fundamentalType.h"

#include <array>

namespace cadise {

template<typename T, std::size_t N>
class TVector {
public:
    static TVector min(const TVector& vA, const TVector& vB);
    static TVector max(const TVector& vA, const TVector& vB);

public:
    TVector();
    explicit TVector(const T v);
    template<typename... Ts>
    TVector(const T vA, const T vB, const Ts... ts);

    TVector(const TVector& other);
    template<typename U>
    explicit TVector(const TVector<U, N>& other);

    TVector operator - () const;
    TVector operator + (const T s) const;
    TVector operator - (const T s) const;
    TVector operator * (const T s) const;
    TVector operator / (const T s) const;
    TVector operator + (const TVector& v) const;
    TVector operator - (const TVector& v) const;
    TVector operator * (const TVector& v) const;
    TVector operator / (const TVector& v) const;
    TVector& operator += (const T s);
    TVector& operator -= (const T s);
    TVector& operator *= (const T s);
    TVector& operator /= (const T s);
    TVector& operator += (const TVector& v);
    TVector& operator -= (const TVector& v);
    TVector& operator *= (const TVector& v);
    TVector& operator /= (const TVector& v);
    TVector& operator  = (const TVector& v);
    T& operator [] (const std::size_t index);
    const T& operator [] (const std::size_t index) const;
    
    template<typename U>
    TVector<U, N> asType() const;

    bool isZero() const;
    bool hasNaN() const;
    bool hasNegative() const;
    bool hasInfinite() const;
    T sum() const;
    T average() const;
    T length() const;
    T lengthSquared() const;
    T product() const;
    T maxComponent() const;
    TVector normalize() const;
    TVector reverse() const;
    TVector square() const;
    TVector complement() const;
    TVector reciprocal() const;
    TVector clamp(const T min, const T max) const;
    std::size_t maxDimension() const;

    void swap(TVector& v);
    bool isEqualTo(const TVector& rhs) const;
    T dot(const TVector& v) const;
    T absDot(const TVector& v) const;
    TVector lerp(const TVector& rhs, const real ratio) const;
    TVector cross(const TVector& v) const;
    TVector reflect(const TVector& normal) const;

    bool canRefract(
        const TVector& normal,
        const real     iorOuter,
        const real     iorInner,
        TVector* const out_vector) const;

    T x() const;
    T y() const;
    T z() const;

private:
    std::array<T, N> _v;
};

// template header implementation

template<typename T, std::size_t N>
inline TVector<T, N> operator + (const T s, const TVector<T, N>& v) {
    return v + s;
}

template<typename T, std::size_t N>
inline TVector<T, N> operator * (const T s, const TVector<T, N>& v) {
    return v * s;
}

} // namespace cadise

#include "math/tVector.ipp"