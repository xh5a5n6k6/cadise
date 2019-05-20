#pragma once

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
    float squaredLength();
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

} // namespace cadise