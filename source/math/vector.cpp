#include "math/vector.h"

#include <algorithm>

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

} // namespace cadise