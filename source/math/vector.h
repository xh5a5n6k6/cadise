#pragma once

#include "math/type.h"

#include <array>

namespace cadise {

template<typename T, uint64 Size>
class Vector {
public:
    Vector();
    Vector(T v);
    template<typename... Ts>
    Vector(T v1, T v2, Ts... ts);

    Vector operator-();
    Vector operator+(const Vector &v);
    Vector operator-(const Vector &v);
    template<typename U>
    Vector operator*(const U s);
    Vector operator*(const Vector &v);
    template<typename U>
    Vector operator/(const U s);
    Vector operator/(const Vector &v);
    Vector& operator+=(const Vector &v);
    Vector& operator-=(const Vector &v);
    template<typename U>
    Vector& operator*=(const U s);
    Vector& operator*=(const Vector &v);
    template<typename U>
    Vector& operator/=(const U s);
    Vector& operator/=(const Vector &v);
    Vector& operator=(const Vector &v);

    bool isZero();
    T length();
    T lengthSquared();
    Vector normalize();
    Vector clamp(T min, T max);

    void swap(Vector &v);
    T dot(Vector v);
    T absDot(Vector v);
    Vector cross(Vector v);
    Vector reflect(Vector normal);

    T x();
    T y();
    T z();

private:
    std::array<T, Size> _v;
};

template<typename T, uint64 Size>
inline Vector<T, Size> operator*(T s, Vector<T, Size> v) {
    return v * s;
}

} // namespace cadise

#include "math/vector.ipp"