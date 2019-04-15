#include "math/algebra.h"

namespace cadise {

/* Vector3 */
Vector3::Vector3() :
    Vector3(0.0f, 0.0f, 0.0f)  {
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
    return std::sqrtf(squaredLength());
}

float Vector3::squaredLength() {
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

float Vector3::x() {
    return _x;
}

float Vector3::y() {
    return _y;
}

float Vector3::z() {
    return _z;
}



/* Matrix4 */
Matrix4::Matrix4() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            _n[i][j] = 0.0f;
}

Matrix4::Matrix4(float n00, float n01, float n02, float n03,
                 float n10, float n11, float n12, float n13,
                 float n20, float n21, float n22, float n23,
                 float n30, float n31, float n32, float n33) {

    _n[0][0] = n00; _n[0][1] = n01; _n[0][2] = n02; _n[0][3] = n03;
    _n[1][0] = n10; _n[1][1] = n11; _n[1][2] = n12; _n[1][3] = n13;
    _n[2][0] = n20; _n[2][1] = n21; _n[2][2] = n22; _n[2][3] = n23;
    _n[3][0] = n30; _n[3][1] = n31; _n[3][2] = n32; _n[3][3] = n33;
}

Matrix4& Matrix4::operator=(const Matrix4 &mat) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            _n[i][j] = mat._n[i][j];

    return *this;
}

Matrix4 Matrix4::transpose() {
    return Matrix4(_n[0][0], _n[1][0], _n[2][0], _n[3][0],
                   _n[0][1], _n[1][1], _n[2][1], _n[3][1],
                   _n[0][2], _n[1][2], _n[2][2], _n[3][2],
                   _n[0][3], _n[1][3], _n[2][3], _n[3][3]);
}

Matrix4 Matrix4::inverse() {
    // Use Gauss-Jordan elimination method
    // First, for each column find a non-zero value to be the diagonal value
    Matrix4 mat = *this;
    Matrix4 inv = Matrix4Identity();
    for (int j = 0; j < 4; j++) {
        // find non-zero row in column j, and assign it as pivot
        int head = -1;
        for (int r = j; r < 4; r++) {
            if (mat._n[r][j] != 0.0f) {
                head = r;
                break;
            }
        }

        if (head == -1) {
            fprintf(stderr, "Inverse matrix doesn't exists !\n");
            exit(0);
        }

        // swap two rows, let pivot be diagonal 
        inv._swapRows(j, head);
        mat._swapRows(j, head);

        // unitize the pivot row
        inv._divideRow(j, mat._n[j][j]);
        mat._divideRow(j, mat._n[j][j]);

        // eliminate off-diagonal elements in column j (from row 0 ~ 3 excluded j)
        for (int r = 0; r < 4; r++) {
            if (r != j) {
                inv._substractRow(r, j, mat._n[r][j]);
                mat._substractRow(r, j, mat._n[r][j]);
            }
        }
    }

    return inv;
}

float Matrix4::n(int i, int j) {
    return _n[i][j];
}

void Matrix4::_swapRows(int r1, int r2) {
    for (int col = 0; col < 4; col++)
        std::swap(_n[r1][col], _n[r2][col]);
}

void Matrix4::_divideRow(int r, float s) {
    for (int col = 0; col < 4; col++)
        _n[r][col] /= s;
}

void Matrix4::_substractRow(int r1, int r2, float s) {
    for (int col = 0; col < 4; col++)
        _n[r1][col] -= _n[r2][col] * s;
}

} // namespace cadise