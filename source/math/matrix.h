#pragma once

#include "math/vector.h"

namespace cadise {

class Matrix4 {
public:
    Matrix4();
    Matrix4(float n00, float n01, float n02, float n03,
            float n10, float n11, float n12, float n13,
            float n20, float n21, float n22, float n23,
            float n30, float n31, float n32, float n33);

    Matrix4& operator=(const Matrix4 &mat);

    Matrix4 transpose();
    Matrix4 inverse();

    Vector3F transformPoint(Vector3F v);
    Vector3F transformVector(Vector3F v);

    float n(int i, int j);

    static Matrix4 identity();
    static Matrix4 scale(float sx, float sy, float sz);
    static Matrix4 translate(float tx, float ty, float tz);
    static Matrix4 lookAt(Vector3F pos, Vector3F tar, Vector3F up);

private:
    // swap two rows
    void _swapRows(int r1, int r2);

    // divide row r with scalar s
    void _divideRow(int r, float s);

    // substract row r1 with row r2 multiplied by scalar s
    // i.e. r1 -= r2*s
    void _substractRow(int r1, int r2, float s);

    float _n[4][4];
};

} // namespace cadise