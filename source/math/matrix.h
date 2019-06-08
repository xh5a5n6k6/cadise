#pragma once

#include "math/vector.h"

namespace cadise {

class Matrix4 {
public:
    Matrix4();
    Matrix4(real n00, real n01, real n02, real n03,
            real n10, real n11, real n12, real n13,
            real n20, real n21, real n22, real n23,
            real n30, real n31, real n32, real n33);

    Matrix4& operator=(const Matrix4 mat);

    Matrix4 transpose();
    Matrix4 inverse();

    Vector3R transformPoint(const Vector3R v);
    Vector3R transformVector(const Vector3R v);

    real n(int32 i, int32 j);

    static Matrix4 identity();
    static Matrix4 scale(real sx, real sy, real sz);
    static Matrix4 translate(real tx, real ty, real tz);
    static Matrix4 lookAt(Vector3R pos, Vector3R tar, Vector3R up);

private:
    // swap two rows
    void _swapRows(int32 r1, int32 r2);

    // divide row r with scalar s
    void _divideRow(int32 r, real s);

    // substract row r1 with row r2 multiplied by scalar s
    // i.e. r1 -= r2*s
    void _substractRow(int32 r1, int32 r2, real s);

    real _n[4][4];
};

} // namespace cadise