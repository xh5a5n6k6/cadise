#pragma once

#include "math/fundamentalType.h"

#include <array>

namespace cadise {

template<typename T, uint32 Size>
class Vector {
public:
    static Vector min(const Vector& v1, const Vector& v2);
    static Vector max(const Vector& v1, const Vector& v2);

public:
    Vector();
    Vector(const T v);
    template<typename... Ts>
    Vector(const T v1, const T v2, const Ts... ts);

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
    T& operator[](const uint32 index);

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
    uint32 maxDimension() const;

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
    std::array<T, Size> _v;
};

// template header implementation

template<typename T, uint32 Size>
inline Vector<T, Size> operator+(const T s, const Vector<T, Size>& v) {
    return v + s;
}

template<typename T, uint32 Size>
inline Vector<T, Size> operator*(const T s, const Vector<T, Size>& v) {
    return v * s;
}

} // namespace cadise

#include "math/vector.ipp"