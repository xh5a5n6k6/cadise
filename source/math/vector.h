#pragma once

#include "math/type.h"

#include <array>

namespace cadise {

class Vector3 {
public:
    Vector3();
    Vector3(float x, float y, float z);

    Vector3 operator-();
    Vector3 operator+(const Vector3 &v);
    Vector3 operator-(const Vector3 &v);
    Vector3 operator*(const float s);
    Vector3 operator*(const Vector3 &v);
    Vector3 operator/(const float s);
    Vector3 operator/(const Vector3 &v);
    Vector3& operator+=(const Vector3 &v);
    Vector3& operator-=(const Vector3 &v);
    Vector3& operator*=(const float s);
    Vector3& operator*=(const Vector3 &v);
    Vector3& operator/=(const float s);
    Vector3& operator/=(const Vector3 &v);
    Vector3& operator=(const Vector3 &v);

    bool isZero();
    float length();
    float lengthSquared();
    Vector3 normalize();
    Vector3 clamp(float min, float max);
    
    void swap(Vector3 &v);
    float dot(Vector3 v);
    float absDot(Vector3 v);
    Vector3 cross(Vector3 v);
    Vector3 reflect(Vector3 normal);

    float x();
    float y();
    float z();

private:
    float _x, _y, _z;
};

inline Vector3 operator*(float s, Vector3 v) {
    return v * s;
}

template<typename T, uint64 Size>
class Vector {
public:
    Vector() = default;
    Vector(T v);
    template<typename... Ts>
    Vector(T v1, T v2, Ts... ts);

    Vector operator-();
    Vector operator+(const Vector &v);
    Vector operator-(const Vector &v);
    Vector operator*(const T s);
    Vector operator*(const Vector &v);
    Vector operator/(const T s);
    Vector operator/(const Vector &v);
    Vector& operator+=(const Vector &v);
    Vector& operator-=(const Vector &v);
    Vector& operator*=(const T s);
    Vector& operator*=(const Vector &v);
    Vector& operator/=(const T s);
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

using Vector2I = Vector<int, 2>;
using Vector3F = Vector<float, 3>;

} // namespace cadise