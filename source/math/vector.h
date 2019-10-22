#pragma once

#include "math/type/fundamentalType.h"

#include <array>

namespace cadise {

template<typename T, std::size_t N>
class Vector {
public:
    static Vector min(const Vector& v1, const Vector& v2);
    static Vector max(const Vector& v1, const Vector& v2);

public:
    Vector();
    Vector(const T v);
    template<typename... Ts>
    Vector(const T v1, const T v2, const Ts... ts);

    template<typename U>
    explicit Vector(const Vector<U, N>& v);

    Vector operator-() const;
    Vector operator+(const T s) const;
    Vector operator-(const T s) const;
    Vector operator*(const T s) const;
    Vector operator/(const T s) const;
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector operator*(const Vector& v) const;
    Vector operator/(const Vector& v) const;
    Vector& operator+=(const T s);
    Vector& operator-=(const T s);
    Vector& operator*=(const T s);
    Vector& operator/=(const T s);
    Vector& operator+=(const Vector& v);
    Vector& operator-=(const Vector& v);
    Vector& operator*=(const Vector& v);
    Vector& operator/=(const Vector& v);
    Vector& operator=(const Vector& v);
    T& operator[](const std::size_t index);
    const T& operator[](const std::size_t index) const;

    bool isZero() const;
    bool hasNaN() const;
    bool hasInfinite() const;
    T sum() const;
    T average() const;
    T length() const;
    T lengthSquared() const;
    T maxComponent() const;
    Vector normalize() const;
    Vector composite() const;
    Vector complement() const;
    Vector reciprocal() const;
    Vector clamp(const T min, const T max) const;
    std::size_t maxDimension() const;

    void swap(Vector& v);
    T dot(const Vector& v) const;
    T absDot(const Vector& v) const;
    Vector lerp(const Vector& rhs, const real ratio) const;
    Vector cross(const Vector& v) const;
    Vector reflect(const Vector& normal) const;
    Vector refract(const Vector& normal, const real iorOuter, const real iorInner) const;

    T x() const;
    T y() const;
    T z() const;

private:
    std::array<T, N> _v;
};

// template header implementation

template<typename T, std::size_t N>
inline Vector<T, N> operator+(const T s, const Vector<T, N>& v) {
    return v + s;
}

template<typename T, std::size_t N>
inline Vector<T, N> operator*(const T s, const Vector<T, N>& v) {
    return v * s;
}

} // namespace cadise

#include "math/vector.ipp"