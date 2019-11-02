#include "math/matrix.h"

#include "math/math.h"
#include "math/vector.h"

namespace cadise {

Matrix4 Matrix4::identity() {
    return Matrix4(1.0_r, 0.0_r, 0.0_r, 0.0_r,
                   0.0_r, 1.0_r, 0.0_r, 0.0_r,
                   0.0_r, 0.0_r, 1.0_r, 0.0_r,
                   0.0_r, 0.0_r, 0.0_r, 1.0_r);
}

Matrix4 Matrix4::scale(const Vector3R& scaleVector) {
    return scale(scaleVector.x(), scaleVector.y(), scaleVector.z());
}

Matrix4 Matrix4::scale(const real sx, const real sy, const real sz) {
    return Matrix4(   sx, 0.0_r, 0.0_r, 0.0_r,
                   0.0_r,    sy, 0.0_r, 0.0_r,
                   0.0_r, 0.0_r,    sz, 0.0_r,
                   0.0_r, 0.0_r, 0.0_r, 1.0_r);
}

Matrix4 Matrix4::translate(const Vector3R& translateVector) {
    return translate(translateVector.x(), translateVector.y(), translateVector.z());
}

Matrix4 Matrix4::translate(const real tx, const real ty, const real tz) {
    return Matrix4(1.0_r, 0.0_r, 0.0_r,    tx,
                   0.0_r, 1.0_r, 0.0_r,    ty,
                   0.0_r, 0.0_r, 1.0_r,    tz,
                   0.0_r, 0.0_r, 0.0_r, 1.0_r);
}

// Return cameraToWorld matrix
Matrix4 Matrix4::lookAt(const Vector3R& position, const Vector3R& direction, const Vector3R& up) {
    Vector3R newZ = direction.composite().normalize();
    Vector3R newX = up.cross(newZ).normalize();
    Vector3R newY = newZ.cross(newX);

    return Matrix4(newX.x(), newY.x(), newZ.x(), position.x(),
                   newX.y(), newY.y(), newZ.y(), position.y(),
                   newX.z(), newY.z(), newZ.z(), position.z(),
                      0.0_r,    0.0_r,    0.0_r,        1.0_r);
}

Matrix4::Matrix4() {
    for (int32 row = 0; row < 4; ++row) {
        for (int32 col = 0; col < 4; ++col) {
            _n[row][col] = 0.0_r;
        }
    }
}

Matrix4::Matrix4(const real n00, const real n01, const real n02, const real n03,
                 const real n10, const real n11, const real n12, const real n13,
                 const real n20, const real n21, const real n22, const real n23,
                 const real n30, const real n31, const real n32, const real n33) {
    _n[0][0] = n00; _n[0][1] = n01; _n[0][2] = n02; _n[0][3] = n03;
    _n[1][0] = n10; _n[1][1] = n11; _n[1][2] = n12; _n[1][3] = n13;
    _n[2][0] = n20; _n[2][1] = n21; _n[2][2] = n22; _n[2][3] = n23;
    _n[3][0] = n30; _n[3][1] = n31; _n[3][2] = n32; _n[3][3] = n33;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
    Matrix4 result;
    for (int32 row = 0; row < 4; ++row) {
        for (int32 col = 0; col < 4; ++col) {
            for (int32 in = 0; in < 4; ++in) {
                result._n[row][col] += _n[row][in] * rhs._n[in][col];
            }
        }
    }

    return result;
}

Matrix4& Matrix4::operator*=(const Matrix4& rhs) {
    Matrix4 tmp;
    for (int32 row = 0; row < 4; ++row) {
        for (int32 col = 0; col < 4; ++col) {
            for (int32 in = 0; in < 4; ++in) {
                tmp._n[row][col] += _n[row][in] * rhs._n[in][col];
            }
        }
    }

    *this = std::move(tmp);
    return *this;
}

Matrix4& Matrix4::operator=(const Matrix4& mat) {
    for (int32 row = 0; row < 4; ++row) {
        for (int32 col = 0; col < 4; ++col) {
            _n[row][col] = mat._n[row][col];
        }
    }

    return *this;
}

Matrix4 Matrix4::transpose() const {
    return Matrix4(_n[0][0], _n[1][0], _n[2][0], _n[3][0],
                   _n[0][1], _n[1][1], _n[2][1], _n[3][1],
                   _n[0][2], _n[1][2], _n[2][2], _n[3][2],
                   _n[0][3], _n[1][3], _n[2][3], _n[3][3]);
}

Matrix4 Matrix4::inverse() const {
    // Use Gauss-Jordan elimination method
    // First, for each column find a non-zero value to be the diagonal value
    Matrix4 mat = *this;
    Matrix4 inv = identity();
    for (int32 col = 0; col < 4; ++col) {
        // find non-zero row in column col, and assign it as pivot
        int32 head = -1;
        for (int32 row = col; row < 4; ++row) {
            if (mat._n[row][col] != 0.0_r) {
                head = row;
                break;
            }
        }

        if (head == -1) {
            exit(-1);
        }

        // swap two rows, let pivot be diagonal 
        inv._swapRows(col, head);
        mat._swapRows(col, head);

        // unitize the pivot row
        inv._divideRow(col, mat._n[col][col]);
        mat._divideRow(col, mat._n[col][col]);

        // eliminate off-diagonal elements in column j (from row 0 ~ 3 excluded j)
        for (int32 row = 0; row < 4; ++row) {
            if (row != col) {
                inv._substractRow(row, col, mat._n[row][col]);
                mat._substractRow(row, col, mat._n[row][col]);
            }
        }
    }

    return inv;
}

Vector3R Matrix4::transformPoint(const Vector3R& v) const {
    return Vector3R(_n[0][0] * v.x() + _n[0][1] * v.y() + _n[0][2] * v.z() + _n[0][3],
                    _n[1][0] * v.x() + _n[1][1] * v.y() + _n[1][2] * v.z() + _n[1][3],
                    _n[2][0] * v.x() + _n[2][1] * v.y() + _n[2][2] * v.z() + _n[2][3]);
}

Vector3R Matrix4::transformVector(const Vector3R& v) const {
    return Vector3R(_n[0][0] * v.x() + _n[0][1] * v.y() + _n[0][2] * v.z(),
                    _n[1][0] * v.x() + _n[1][1] * v.y() + _n[1][2] * v.z(),
                    _n[2][0] * v.x() + _n[2][1] * v.y() + _n[2][2] * v.z());
}

real Matrix4::n(const int32 row, const int32 col) const {
    return _n[row][col];
}

void Matrix4::_swapRows(const int32 r1, const int32 r2) {
    for (int32 col = 0; col < 4; ++col) {
        math::swap(_n[r1][col], _n[r2][col]);
    }
}

void Matrix4::_divideRow(const int32 r, const real s) {
    for (int32 col = 0; col < 4; ++col) {
        _n[r][col] /= s;
    }
}

void Matrix4::_substractRow(const int32 r1, const int32 r2, const real s) {
    for (int32 col = 0; col < 4; ++col) {
        _n[r1][col] -= _n[r2][col] * s;
    }
}

} // namespace cadise