#include "math/matrix.h"

namespace cadise {

Matrix4::Matrix4() {
    for (int32 i = 0; i < 4; i++) {
        for (int32 j = 0; j < 4; j++) {
            _n[i][j] = 0.0_r;
        }
    }
}

Matrix4::Matrix4(real n00, real n01, real n02, real n03,
                 real n10, real n11, real n12, real n13,
                 real n20, real n21, real n22, real n23,
                 real n30, real n31, real n32, real n33) {

    _n[0][0] = n00; _n[0][1] = n01; _n[0][2] = n02; _n[0][3] = n03;
    _n[1][0] = n10; _n[1][1] = n11; _n[1][2] = n12; _n[1][3] = n13;
    _n[2][0] = n20; _n[2][1] = n21; _n[2][2] = n22; _n[2][3] = n23;
    _n[3][0] = n30; _n[3][1] = n31; _n[3][2] = n32; _n[3][3] = n33;
}

Matrix4& Matrix4::operator=(const Matrix4 mat) {
    for (int32 i = 0; i < 4; i++) {
        for (int32 j = 0; j < 4; j++) {
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
    for (int32 j = 0; j < 4; j++) {
        // find non-zero row in column j, and assign it as pivot
        int32 head = -1;
        for (int32 r = j; r < 4; r++) {
            if (mat._n[r][j] != 0.0_r) {
                head = r;
                break;
            }
        }

        if (head == -1) {
            exit(-1);
        }

        // swap two rows, let pivot be diagonal 
        inv._swapRows(j, head);
        mat._swapRows(j, head);

        // unitize the pivot row
        inv._divideRow(j, mat._n[j][j]);
        mat._divideRow(j, mat._n[j][j]);

        // eliminate off-diagonal elements in column j (from row 0 ~ 3 excluded j)
        for (int32 r = 0; r < 4; r++) {
            if (r != j) {
                inv._substractRow(r, j, mat._n[r][j]);
                mat._substractRow(r, j, mat._n[r][j]);
            }
        }
    }

    return inv;
}

Vector3R Matrix4::transformPoint(Vector3R v) {
    return Vector3R(_n[0][0] * v.x() + _n[0][1] * v.y() + _n[0][2] * v.z() + _n[0][3],
                    _n[1][0] * v.x() + _n[1][1] * v.y() + _n[1][2] * v.z() + _n[1][3],
                    _n[2][0] * v.x() + _n[2][1] * v.y() + _n[2][2] * v.z() + _n[2][3]);
}

Vector3R Matrix4::transformVector(Vector3R v) {
    return Vector3R(_n[0][0] * v.x() + _n[0][1] * v.y() + _n[0][2] * v.z(),
                    _n[1][0] * v.x() + _n[1][1] * v.y() + _n[1][2] * v.z(),
                    _n[2][0] * v.x() + _n[2][1] * v.y() + _n[2][2] * v.z());
}

real Matrix4::n(int32 i, int32 j) {
    return _n[i][j];
}

Matrix4 Matrix4::identity() {
    return Matrix4(1.0_r, 0.0_r, 0.0_r, 0.0_r,
                   0.0_r, 1.0_r, 0.0_r, 0.0_r,
                   0.0_r, 0.0_r, 1.0_r, 0.0_r,
                   0.0_r, 0.0_r, 0.0_r, 1.0_r);
}

Matrix4 Matrix4::scale(real sx, real sy, real sz) {
    return Matrix4(   sx, 0.0_r, 0.0_r, 0.0_r,
                   0.0_r,    sy, 0.0_r, 0.0_r,
                   0.0_r, 0.0_r,    sz, 0.0_r,
                   0.0_r, 0.0_r, 0.0_r, 1.0_r);
}

Matrix4 Matrix4::translate(real tx, real ty, real tz) {
    return Matrix4(1.0_r, 0.0_r, 0.0_r,   tx,
                   0.0_r, 1.0_r, 0.0_r,   ty,
                   0.0_r, 0.0_r, 1.0_r,   tz,
                   0.0_r, 0.0_r, 0.0_r, 1.0_r);
}

// Return cameraToWorld matrix
Matrix4 Matrix4::lookAt(Vector3R pos, Vector3R tar, Vector3R up) {
    Vector3R newZ = (pos - tar).normalize();
    Vector3R newX = up.cross(newZ).normalize();
    Vector3R newY = newZ.cross(newX);

    return Matrix4(newX.x(), newY.x(), newZ.x(), pos.x(),
                   newX.y(), newY.y(), newZ.y(), pos.y(),
                   newX.z(), newY.z(), newZ.z(), pos.z(),
                      0.0_r,    0.0_r,    0.0_r,   1.0_r);
}

void Matrix4::_swapRows(int32 r1, int32 r2) {
    for (int32 col = 0; col < 4; col++) {
        std::swap(_n[r1][col], _n[r2][col]);
    }
}

void Matrix4::_divideRow(int32 r, real s) {
    for (int32 col = 0; col < 4; col++) {
        _n[r][col] /= s;
    }
}

void Matrix4::_substractRow(int32 r1, int32 r2, real s) {
    for (int32 col = 0; col < 4; col++) {
        _n[r1][col] -= _n[r2][col] * s;
    }
}

} // namespace cadise