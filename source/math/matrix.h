#pragma once

#include "math/type/mathType.h"

namespace cadise {

class Matrix4 {
public:
    static Matrix4 identity();
    static Matrix4 scale(const Vector3R& scaleVector);
    static Matrix4 scale(const real sx, const real sy, const real sz);  
    static Matrix4 translate(const Vector3R& translateVector);
    static Matrix4 translate(const real tx, const real ty, const real tz);
    static Matrix4 lookAt(const Vector3R& position, const Vector3R& direction, const Vector3R& up);

public:
    Matrix4();
    Matrix4(const real n00, const real n01, const real n02, const real n03,
            const real n10, const real n11, const real n12, const real n13,
            const real n20, const real n21, const real n22, const real n23,
            const real n30, const real n31, const real n32, const real n33);

    Matrix4 operator*(const Matrix4& rhs) const;
    Matrix4& operator*=(const Matrix4& rhs);
    Matrix4& operator=(const Matrix4& rhs);

    Matrix4 transpose() const;
    Matrix4 inverse() const;

    void transformPoint(const Vector3R& p, Vector3R* const out_point) const;
    void transformVector(const Vector3R& v, Vector3R* const out_vector) const;

    real n(const std::size_t row, const std::size_t col) const;

private:
    // swap two rows
    void _swapRows(const std::size_t r1, const std::size_t r2);

    // divide row r with scalar s
    void _divideRow(const std::size_t r, const real s);

    // substract row r1 with row r2 multiplied by scalar s
    // i.e. r1 -= r2*s
    void _substractRow(const std::size_t r1, const std::size_t r2, const real s);

    real _n[4][4];
};

} // namespace cadise