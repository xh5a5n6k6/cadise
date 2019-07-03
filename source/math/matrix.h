#pragma once

#include "math/vector.h"

namespace cadise {

class Matrix4 {
public:
    Matrix4();
    Matrix4(const real n00, const real n01, const real n02, const real n03,
            const real n10, const real n11, const real n12, const real n13,
            const real n20, const real n21, const real n22, const real n23,
            const real n30, const real n31, const real n32, const real n33);

    Matrix4& operator=(const Matrix4 mat);

    Matrix4 transpose() const;
    Matrix4 inverse() const;

    Vector3R transformPoint(const Vector3R v) const;
    Vector3R transformVector(const Vector3R v) const;

    real n(const int32 row, const int32 col) const;

    static Matrix4 identity();
    static Matrix4 scale(const real sx, const real sy, const real sz);
    static Matrix4 translate(const real tx, const real ty, const real tz);
    static Matrix4 lookAt(const Vector3R pos, const Vector3R tar, const Vector3R up);

private:
    // swap two rows
    void _swapRows(const int32 r1, const int32 r2);

    // divide row r with scalar s
    void _divideRow(const int32 r, const real s);

    // substract row r1 with row r2 multiplied by scalar s
    // i.e. r1 -= r2*s
    void _substractRow(const int32 r1, const int32 r2, const real s);

    real _n[4][4];
};

} // namespace cadise