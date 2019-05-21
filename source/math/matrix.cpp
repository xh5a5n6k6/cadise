#include "math/matrix.h"

#include "math/vector.h"

#include <algorithm>
#include <iostream>

namespace cadise {

Matrix4::Matrix4() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _n[i][j] = 0.0f;
        }
    }
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
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            _n[i][j] = mat._n[i][j];
        }
    }

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
    Matrix4 inv = identity();
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
            std::cerr << "Inverse matrix doesn't exists !" << std::endl;
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

Vector3 Matrix4::transformPoint(Vector3 v) {
    return Vector3(_n[0][0] * v.x() + _n[0][1] * v.y() + _n[0][2] * v.z() + _n[0][3],
                   _n[1][0] * v.x() + _n[1][1] * v.y() + _n[1][2] * v.z() + _n[1][3],
                   _n[2][0] * v.x() + _n[2][1] * v.y() + _n[2][2] * v.z() + _n[2][3]);
}

Vector3 Matrix4::transformVector(Vector3 v) {
    return Vector3(_n[0][0] * v.x() + _n[0][1] * v.y() + _n[0][2] * v.z(),
                   _n[1][0] * v.x() + _n[1][1] * v.y() + _n[1][2] * v.z(),
                   _n[2][0] * v.x() + _n[2][1] * v.y() + _n[2][2] * v.z());
}

Matrix4 Matrix4::identity() {
    return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                   0.0f, 1.0f, 0.0f, 0.0f,
                   0.0f, 0.0f, 1.0f, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 Matrix4::scale(float sx, float sy, float sz) {
    return Matrix4(  sx, 0.0f, 0.0f, 0.0f,
                   0.0f,   sy, 0.0f, 0.0f,
                   0.0f, 0.0f,   sz, 0.0f,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

Matrix4 Matrix4::translate(float tx, float ty, float tz) {
    return Matrix4(1.0f, 0.0f, 0.0f,   tx,
                   0.0f, 1.0f, 0.0f,   ty,
                   0.0f, 0.0f, 1.0f,   tz,
                   0.0f, 0.0f, 0.0f, 1.0f);
}

// Return cameraToWorld matrix
Matrix4 Matrix4::lookAt(Vector3 pos, Vector3 tar, Vector3 up) {
    Vector3 newZ = (pos - tar).normalize();
    Vector3 newX = up.cross(newZ).normalize();
    Vector3 newY = newZ.cross(newX);

    return Matrix4(newX.x(), newY.x(), newZ.x(), pos.x(),
                   newX.y(), newY.y(), newZ.y(), pos.y(),
                   newX.z(), newY.z(), newZ.z(), pos.z(),
                       0.0f,     0.0f,     0.0f,    1.0f);
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

float Matrix4::n(int i, int j) {
    return _n[i][j];
}

} // namespace cadise