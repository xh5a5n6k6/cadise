#pragma once

#include "math/type.h"

#include <array>

namespace cadise {

template<typename T, uint64 Size>
class Vector;

using Vector2I = Vector<int, 2>;
using Vector3F = Vector<float, 3>;

template<typename T, uint64 Size>
class Vector {
public:
    Vector();
    Vector(T v);
    template<typename... Ts>
    Vector(T v1, T v2, Ts... ts);

    Vector operator-();
    template<typename U>
    Vector operator+(const U s);
    Vector operator+(const Vector &v);
    template<typename U>
    Vector operator-(const U s);
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
    Vector reciprocal();
    Vector clamp(T min, T max);

    void swap(Vector &v);
    T dot(Vector v);
    T absDot(Vector v);
    Vector cross(Vector v);
    Vector reflect(Vector normal);

    T x();
    T y();
    T z();

    static Vector min(Vector v1, Vector v2);
    static Vector max(Vector v1, Vector v2);

private:
    std::array<T, Size> _v;
};

template<typename T, uint64 Size>
inline Vector<T, Size> operator*(T s, Vector<T, Size> v) {
    return v * s;
}

} // namespace cadise

#include "math/vector.ipp"