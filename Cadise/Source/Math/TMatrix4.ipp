#pragma once

#include "Math/TMatrix4.h"

#include "Foundation/Assertion.h"
#include "Math/TVector3.h"

#include <utility>

namespace cadise
{

template<typename T>
inline TMatrix4<T> TMatrix4<T>::makeIdentity() 
{
    return TMatrix4<T>(
        T(1), T(0), T(0), T(0),
        T(0), T(1), T(0), T(0),
        T(0), T(0), T(1), T(0),
        T(0), T(0), T(0), T(1));
}

template<typename T>
inline TMatrix4<T> TMatrix4<T>::makeScale(const TVector3<T>& scaleVector)
{
    return TMatrix4<T>::makeScale(
        scaleVector.x(), 
        scaleVector.y(), 
        scaleVector.z());
}

template<typename T>
inline TMatrix4<T> TMatrix4<T>::makeScale(const T sx, const T sy, const T sz)
{
    return TMatrix4<T>(
          sx, T(0), T(0), T(0),
        T(0),   sy, T(0), T(0),
        T(0), T(0),   sz, T(0),
        T(0), T(0), T(0), T(1));
}

template<typename T>
inline TMatrix4<T> TMatrix4<T>::makeTranslate(const TVector3<T>& translateVector)
{
    return TMatrix4<T>::makeTranslate(
        translateVector.x(), 
        translateVector.y(), 
        translateVector.z());
}

template<typename T>
inline TMatrix4<T> TMatrix4<T>::makeTranslate(const T tx, const T ty, const T tz) 
{
    return TMatrix4<T>(
        T(1), T(0), T(0),   tx,
        T(0), T(1), T(0),   ty,
        T(0), T(0), T(1),   tz,
        T(0), T(0), T(0), T(1));
}

// Return cameraToWorld matrix
template<typename T>
inline TMatrix4<T> TMatrix4<T>::makeLookAt(
    const TVector3<T>& position, 
    const TVector3<T>& direction, 
    const TVector3<T>& up)
{
    const TVector3<T> newZ = direction.negate().normalize();
    const TVector3<T> newX = up.cross(newZ).normalize();
    const TVector3<T> newY = newZ.cross(newX);

    return TMatrix4<T>(
        newX.x(), newY.x(), newZ.x(), position.x(),
        newX.y(), newY.y(), newZ.y(), position.y(),
        newX.z(), newY.z(), newZ.z(), position.z(),
            T(0),     T(0),     T(0),         T(1));
}

template<typename T>
inline TMatrix4<T>::TMatrix4() 
{
    for (std::size_t row = 0; row < 4; ++row)
    {
        _n[row].fill(static_cast<T>(0));
    }
}

template<typename T>
inline TMatrix4<T>::TMatrix4(
    const T n00, const T n01, const T n02, const T n03,
    const T n10, const T n11, const T n12, const T n13,
    const T n20, const T n21, const T n22, const T n23,
    const T n30, const T n31, const T n32, const T n33) 
{
    _n[0][0] = n00; _n[0][1] = n01; _n[0][2] = n02; _n[0][3] = n03;
    _n[1][0] = n10; _n[1][1] = n11; _n[1][2] = n12; _n[1][3] = n13;
    _n[2][0] = n20; _n[2][1] = n21; _n[2][2] = n22; _n[2][3] = n23;
    _n[3][0] = n30; _n[3][1] = n31; _n[3][2] = n32; _n[3][3] = n33;
}

template<typename T>
inline TMatrix4<T>::TMatrix4(const TMatrix4<T>& other) = default;

template<typename T>
inline TMatrix4<T> TMatrix4<T>::operator * (const TMatrix4<T>& rhs) const
{
    return this->mul(rhs);
}

template<typename T>
inline TMatrix4<T> TMatrix4<T>::mul(const TMatrix4<T>& rhs) const
{
    TMatrix4<T> result;
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t col = 0; col < 4; ++col) 
        {
            for (std::size_t in = 0; in < 4; ++in) 
            {
                result._n[row][col] += _n[row][in] * rhs._n[in][col];
            }
        }
    }

    return result;
}

template<typename T>
inline TMatrix4<T>& TMatrix4<T>::mulLocal(const TMatrix4<T>& rhs)
{
    TMatrix4<T> tmp;
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t col = 0; col < 4; ++col)
        {
            for (std::size_t in = 0; in < 4; ++in)
            {
                tmp._n[row][col] += _n[row][in] * rhs._n[in][col];
            }
        }
    }

    *this = std::move(tmp);

    return *this;
}

template<typename T>
inline TMatrix4<T> TMatrix4<T>::transpose() const 
{
    return TMatrix4<T>(
        _n[0][0], _n[1][0], _n[2][0], _n[3][0],
        _n[0][1], _n[1][1], _n[2][1], _n[3][1],
        _n[0][2], _n[1][2], _n[2][2], _n[3][2],
        _n[0][3], _n[1][3], _n[2][3], _n[3][3]);
}

template<typename T>
inline TMatrix4<T> TMatrix4<T>::inverse() const
{
    TMatrix4<T> mat = *this;
    TMatrix4<T> inv = TMatrix4<T>::makeIdentity();

    // use Gauss-Jordan elimination method
    // for each column find a non-zero value to be the diagonal value
    for (std::size_t col = 0; col < 4; ++col)
    {
        // find non-zero row in column col, and assign it as pivot
        std::size_t head = 4;
        for (std::size_t row = col; row < 4; ++row)
        {
            if (mat._n[row][col] != T(0))
            {
                head = row;

                break;
            }
        }

        if (head == 4)
        {
            // TODO: logging
            return TMatrix4<T>::makeIdentity();
        }

        // swap two rows, let pivot be diagonal 
        inv._swapRows(col, head);
        mat._swapRows(col, head);

        // unitize the pivot row
        inv._divideRow(col, mat._n[col][col]);
        mat._divideRow(col, mat._n[col][col]);

        // eliminate off-diagonal elements in column j (from row 0 ~ 3 excluded j)
        for (std::size_t row = 0; row < 4; ++row)
        {
            if (row != col)
            {
                inv._substractRow(row, col, mat._n[row][col]);
                mat._substractRow(row, col, mat._n[row][col]);
            }
        }
    }

    return inv;
}

template<typename T>
inline void TMatrix4<T>::transformPoint(
    const TVector3<T>& point, 
    TVector3<T>* const out_point) const 
{
    CS_ASSERT(out_point);

    out_point->set(
        _n[0][0] * point.x() + _n[0][1] * point.y() + _n[0][2] * point.z() + _n[0][3],
        _n[1][0] * point.x() + _n[1][1] * point.y() + _n[1][2] * point.z() + _n[1][3],
        _n[2][0] * point.x() + _n[2][1] * point.y() + _n[2][2] * point.z() + _n[2][3]);
}

template<typename T>
inline void TMatrix4<T>::transformVector(
    const TVector3<T>& vector, 
    TVector3<T>* const out_vector) const
{
    CS_ASSERT(out_vector);

    out_vector->set(
        _n[0][0] * vector.x() + _n[0][1] * vector.y() + _n[0][2] * vector.z(),
        _n[1][0] * vector.x() + _n[1][1] * vector.y() + _n[1][2] * vector.z(),
        _n[2][0] * vector.x() + _n[2][1] * vector.y() + _n[2][2] * vector.z());
}

template<typename T>
inline T TMatrix4<T>::n(const std::size_t row, const std::size_t col) const 
{
    return _n[row][col];
}

template<typename T>
inline void TMatrix4<T>::_swapRows(const std::size_t rowA, const std::size_t rowB) 
{
    for (std::size_t col = 0; col < 4; ++col) 
    {
        std::swap(_n[rowA][col], _n[rowB][col]);
    }
}

template<typename T>
inline void TMatrix4<T>::_divideRow(const std::size_t row, const T scalar) 
{
    const T rcpScalar = T(1) / scalar;

    for (std::size_t col = 0; col < 4; ++col)
    {
        _n[row][col] *= rcpScalar;
    }
}

template<typename T>
inline void TMatrix4<T>::_substractRow(const std::size_t rowA, const std::size_t rowB, const T scalar)
{
    for (std::size_t col = 0; col < 4; ++col)
    {
        _n[rowA][col] -= _n[rowB][col] * scalar;
    }
}

} // namespace cadise