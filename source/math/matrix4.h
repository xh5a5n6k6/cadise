#pragma once

#include "math/type/mathType.h"

#include <array>

namespace cadise {

template<typename T>
class Matrix4 {
public:
    static Matrix4 identity();
    static Matrix4 scale(const Vector<T, 3>& scaleVector);
    static Matrix4 scale(const T sx, const T sy, const T sz);  
    static Matrix4 translate(const Vector<T, 3>& translateVector);
    static Matrix4 translate(const T tx, const T ty, const T tz);
    static Matrix4 lookAt(
        const Vector<T, 3>& position, 
        const Vector<T, 3>& direction, 
        const Vector<T, 3>& up);

public:
    Matrix4();
    Matrix4(
        const T n00, const T n01, const T n02, const T n03,
        const T n10, const T n11, const T n12, const T n13,
        const T n20, const T n21, const T n22, const T n23,
        const T n30, const T n31, const T n32, const T n33);
    Matrix4(const Matrix4& other);

    Matrix4 operator*(const Matrix4& rhs) const;
    Matrix4& operator*=(const Matrix4& rhs);
    Matrix4& operator=(const Matrix4& rhs);

    Matrix4 transpose() const;
    Matrix4 inverse() const;

    void transformPoint(const Vector<T, 3>& p, Vector<T, 3>* const out_point) const;
    void transformVector(const Vector<T, 3>& v, Vector<T, 3>* const out_vector) const;

    T n(const std::size_t row, const std::size_t col) const;

private:
    // swap two rows
    void _swapRows(const std::size_t r1, const std::size_t r2);

    // divide row r with scalar s
    void _divideRow(const std::size_t r, const T s);

    // substract row r1 with row r2 multiplied by scalar s
    // i.e. r1 -= r2*s
    void _substractRow(const std::size_t r1, const std::size_t r2, const T s);

    std::array<std::array<T, 4>, 4> _n;
};

} // namespace cadise

#include "math/matrix4.ipp"