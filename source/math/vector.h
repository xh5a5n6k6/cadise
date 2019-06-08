#pragma once

#include "math/type.h"

#include <array>

namespace cadise {

template<typename T, uint32 Size>
class Vector;

using Vector2I = Vector<int32, 2>;
using Vector3R = Vector<real, 3>;

template<typename T, uint32 Size>
class Vector {
public:
    Vector();
    Vector(T v);
    template<typename... Ts>
    Vector(T v1, T v2, Ts... ts);

    Vector operator-();
    Vector operator+(const T s);
    Vector operator+(const Vector v);
    Vector operator-(const T s);
    Vector operator-(const Vector v);
    Vector operator*(const T s);
    Vector operator*(const Vector v);
    Vector operator/(const T s);
    Vector operator/(const Vector v);
    Vector& operator+=(const Vector v);
    Vector& operator-=(const Vector v);
    Vector& operator*=(const T s);
    Vector& operator*=(const Vector v);
    Vector& operator/=(const T s);
    Vector& operator/=(const Vector v);
    Vector& operator=(const Vector v);

    bool isZero();
    T length();
    T lengthSquared();
    Vector normalize();
    Vector reciprocal();
    Vector clamp(const T min, const T max);

    void swap(Vector &v);
    T dot(const Vector v);
    T absDot(const Vector v);
    Vector cross(const Vector v);
    Vector reflect(const Vector normal);

    T x();
    T y();
    T z();

    static Vector min(const Vector v1, const Vector v2);
    static Vector max(const Vector v1, const Vector v2);

private:
    std::array<T, Size> _v;
};

template<typename T, uint32 Size>
inline Vector<T, Size> operator+(const T s, Vector<T, Size> v) {
    return v + s;
}

template<typename T, uint32 Size>
inline Vector<T, Size> operator*(const T s, Vector<T, Size> v) {
    return v * s;
}

} // namespace cadise

#include "math/vector.ipp"