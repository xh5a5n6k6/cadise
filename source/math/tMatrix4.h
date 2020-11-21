#pragma once

#include "math/type/mathType.h"

#include <array>

namespace cadise {

template<typename T>
class TMatrix4 {
public:
    static TMatrix4 identity();
    static TMatrix4 scale(const TVector<T, 3>& scaleVector);
    static TMatrix4 scale(const T sx, const T sy, const T sz);  
    static TMatrix4 translate(const TVector<T, 3>& translateVector);
    static TMatrix4 translate(const T tx, const T ty, const T tz);
    static TMatrix4 lookAt(
        const TVector<T, 3>& position, 
        const TVector<T, 3>& direction, 
        const TVector<T, 3>& up);

public:
    TMatrix4();
    TMatrix4(
        const T n00, const T n01, const T n02, const T n03,
        const T n10, const T n11, const T n12, const T n13,
        const T n20, const T n21, const T n22, const T n23,
        const T n30, const T n31, const T n32, const T n33);
    TMatrix4(const TMatrix4& other);

    TMatrix4 operator * (const TMatrix4& rhs) const;
    TMatrix4& operator *= (const TMatrix4& rhs);
    TMatrix4& operator  = (const TMatrix4& rhs);

    TMatrix4 transpose() const;
    TMatrix4 inverse() const;

    void transformPoint(const TVector<T, 3>& p, TVector<T, 3>* const out_point) const;
    void transformVector(const TVector<T, 3>& v, TVector<T, 3>* const out_vector) const;

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

#include "math/tMatrix4.ipp"