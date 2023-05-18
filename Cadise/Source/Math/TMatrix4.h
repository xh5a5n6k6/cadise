#pragma once

#include "Math/Type/MathType.h"

#include <array>

namespace cadise
{

template<typename T>
class TMatrix4
{
public:
    static TMatrix4 makeIdentity();
    static TMatrix4 makeScale(const TVector3<T>& scaleVector);
    static TMatrix4 makeScale(const T sx, const T sy, const T sz);
    static TMatrix4 makeTranslate(const TVector3<T>& translateVector);
    static TMatrix4 makeTranslate(const T tx, const T ty, const T tz);

public:
    TMatrix4();
    TMatrix4(
        const T n00, const T n01, const T n02, const T n03,
        const T n10, const T n11, const T n12, const T n13,
        const T n20, const T n21, const T n22, const T n23,
        const T n30, const T n31, const T n32, const T n33);
    TMatrix4(const TMatrix4& other);

    TMatrix4 operator * (const TMatrix4& rhs) const;

    TMatrix4  mul(const TMatrix4& rhs) const;
    TMatrix4& mulLocal(const TMatrix4& rhs);

    TMatrix4 transpose() const;
    TMatrix4 inverse() const;

    void transformPoint(
        const TVector3<T>& point,
        TVector3<T>* const out_point) const;

    void transformVector(
        const TVector3<T>& vector,
        TVector3<T>* const out_vector) const;

    T n(const std::size_t row, const std::size_t col) const;

private:
    void _swapRows(const std::size_t rowA, const std::size_t rowB);
    void _divideRow(const std::size_t row, const T scalar);

    // substract rowA with rowB multiplied by scalar
    // i.e. r1 -= r2*s
    void _substractRow(const std::size_t rowA, const std::size_t rowB, const T scalar);

    std::array<std::array<T, 4>, 4> _n;
};

} // namespace cadise

#include "Math/TMatrix4.ipp"