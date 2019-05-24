#include "math/vector.h"

#include <algorithm>
#include <cassert>

namespace cadise {

Vector3::Vector3() :
    Vector3(0.0f, 0.0f, 0.0f) {
}

Vector3::Vector3(float x, float y, float z) :
    _x(x), _y(y), _z(z) {
}

Vector3 Vector3::operator-() {
    return Vector3(-_x, -_y, -_z);
}

Vector3 Vector3::operator+(const Vector3 &v) {
    return Vector3(_x + v._x, _y + v._y, _z + v._z);
}

Vector3 Vector3::operator-(const Vector3 &v) {
    return Vector3(_x - v._x, _y - v._y, _z - v._z);
}

Vector3 Vector3::operator*(const float s) {
    return Vector3(s*_x, s*_y, s*_z);
}

Vector3 Vector3::operator*(const Vector3 &v) {
    return Vector3(_x * v._x, _y * v._y, _z * v._z);
}

Vector3 Vector3::operator/(const float s) {
    float invS = 1.0f / s;
    return *this * invS;
}

Vector3 Vector3::operator/(const Vector3 &v) {
    return Vector3(_x / v._x, _y / v._y, _z / v._z);
}

Vector3& Vector3::operator+=(const Vector3 &v) {
    _x += v._x; _y += v._y; _z += v._z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3 &v) {
    _x -= v._x; _y -= v._y; _z -= v._z;
    return *this;
}

Vector3& Vector3::operator*=(const float s) {
    _x *= s; _y *= s; _z *= s;
    return *this;
}

Vector3& Vector3::operator*=(const Vector3 &v) {
    _x *= v._x; _y *= v._y; _z *= v._z;
    return *this;
}

Vector3& Vector3::operator/=(const float s) {
    float invS = 1.0f / s;
    _x *= invS; _y *= invS; _z *= invS;
    return *this;
}

Vector3& Vector3::operator/=(const Vector3 &v) {
    _x /= v._x; _y /= v._y; _z /= v._z;
    return *this;
}

Vector3& Vector3::operator=(const Vector3 &v) {
    _x = v._x; _y = v._y; _z = v._z;
    return *this;
}

bool Vector3::isZero() {
    return (_x == 0.0f) && (_y == 0.0f) && (_z == 0.0f);
}

float Vector3::length() {
    return std::sqrtf(lengthSquared());
}

float Vector3::lengthSquared() {
    return _x * _x + _y * _y + _z * _z;
}

Vector3 Vector3::normalize() {
    float invLength = 1.0f / length();
    return *this * invLength;
}

Vector3 Vector3::clamp(float min, float max) {
    return Vector3(std::clamp(_x, min, max),
                   std::clamp(_y, min, max),
                   std::clamp(_z, min, max));
}

void Vector3::swap(Vector3 &v) {
    std::swap(_x, v._x);
    std::swap(_y, v._y);
    std::swap(_z, v._z);
}

float Vector3::dot(Vector3 v) {
    return _x * v._x + _y * v._y + _z * v._z;
}

float Vector3::absDot(Vector3 v) {
    return std::abs(dot(v));
}

Vector3 Vector3::cross(Vector3 v) {
    return Vector3(_y*v._z - _z*v._y,
                   _z*v._x - _x*v._z,
                   _x*v._y - _y*v._x);
}

Vector3 Vector3::reflect(Vector3 normal) {
    Vector3 result = 2.0f * absDot(normal) * normal;
    return result - *this;
}

float Vector3::x() {
    return _x;
}

float Vector3::y() {
    return _y;
}

float Vector3::z() {
    return _z;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::operator-() {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = -_v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::operator+(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) + v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::operator-(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) - v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::operator*(const T s) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) * s;
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::operator*(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) * v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::operator/(const T s) {
    assert(s != static_cast<T>(0));

    T invS = static_cast<T>(1) / s;
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = _v.at(i) * invS;
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::operator/(const Vector<T, Size> &v) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        assert(v._v.at(i) != static_cast<T>(0));

        result._v.at(i) = _v.at(i) / v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size>& Vector<T, Size>::operator+=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) += v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
Vector<T, Size>& Vector<T, Size>::operator-=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) -= v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
Vector<T, Size>& Vector<T, Size>::operator*=(const T s) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) *= s;
    }

    return *this;
}

template<typename T, uint64 Size>
Vector<T, Size>& Vector<T, Size>::operator*=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) *= v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
Vector<T, Size>& Vector<T, Size>::operator/=(const T s) {
    assert(s != static_cast<T>(0));

    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) /= s;
    }

    return *this;
}

template<typename T, uint64 Size>
Vector<T, Size>& Vector<T, Size>::operator/=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        assert(v._v.at(i) != static_cast<T>(0));

        _v.at(i) /= v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
Vector<T, Size>& Vector<T, Size>::operator=(const Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) = v._v.at(i);
    }

    return *this;
}

template<typename T, uint64 Size>
Vector<T, Size>::Vector(T v) {
    for (uint64 i = 0; i < Size; i++) {
        _v.at(i) = v;
    }
}

template<typename T, uint64 Size>
template<typename... Ts>
Vector<T, Size>::Vector(T v1, T v2, Ts... ts) :
    _v({ v1, v2, ts... }) {
}

template<typename T, uint64 Size>
bool Vector<T, Size>::isZero() {
    bool result = true;
    for (uint64 i = 0; i < Size; i++) {
        result &= _v.at(i) == static_cast<T>(0);
    }

    return result;
}

template<typename T, uint64 Size>
T Vector<T, Size>::length() {
    return std::sqrt(lengthSquared());
}

template<typename T, uint64 Size>
T Vector<T, Size>::lengthSquared() {
    T result = static_cast<T>(0);
    for (uint64 i = 0; i < Size; i++) {
        result += _v.at(i) * _v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::normalize() {
    assert(length() > static_cast<T>(0));

    T invLength = static_cast<T>(1) / length();
    return *this * invLength;
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::clamp(T min, T max) {
    Vector<T, Size> result;
    for (uint64 i = 0; i < Size; i++) {
        result._v.at(i) = std::clamp(_v.at(i), min, max);
    }

    return result;
}

template<typename T, uint64 Size>
void Vector<T, Size>::swap(Vector<T, Size> &v) {
    for (uint64 i = 0; i < Size; i++) {
        std::swap(_v.at(i), v._v.at(i));
    }
}

template<typename T, uint64 Size>
T Vector<T, Size>::dot(Vector<T, Size> v) {
    T result = static_cast<T>(0);
    for (uint64 i = 0; i < Size; i++) {
        result += _v.at(i) * v._v.at(i);
    }

    return result;
}

template<typename T, uint64 Size>
T Vector<T, Size>::absDot(Vector<T, Size> v) {
    return std::abs(dot(v));
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::cross(Vector<T, Size> v) {
    static_assert(Size == 3, "Error in vector's cross, this vector doesn't support cross method\n");

    return Vector<T, Size>(_v.at(1) * v._v.at(2) - _v.at(2) * v._v.at(1),
                           _v.at(2) * v._v.at(0) - _v.at(0) * v._v.at(2), 
                           _v.at(0) * v._v.at(1) - _v.at(1) * v._v.at(0));
}

template<typename T, uint64 Size>
Vector<T, Size> Vector<T, Size>::reflect(Vector<T, Size> v) {
    Vector<T, Size> result = static_cast<T>(2) * absDot(normal) * normal;
    return result - *this;
}

template<typename T, uint64 Size>
T Vector<T, Size>::x() {
    return _v.at(0);
}

template<typename T, uint64 Size>
T Vector<T, Size>::y() {
    return _v.at(1);
}

template<typename T, uint64 Size>
T Vector<T, Size>::z() {
    static_assert(Size > 2, "Error in vector's z, this vector doesn't support z value\n");

    return _v.at(2);
}

} // namespace cadise