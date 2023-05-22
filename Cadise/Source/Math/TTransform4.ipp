#pragma once

#include "Math/TTransform4.h"

#include "Foundation/Assertion.h"
#include "Math/TVector3.h"

namespace cadise
{

template<typename T>
inline TTransform4<T>::TTransform4() :
    TTransform4(TMatrix4<T>::makeIdentity(), TMatrix4<T>::makeIdentity())
{}

template<typename T>
inline TTransform4<T>::TTransform4(const TMatrix4<T>& matrix) :
    TTransform4(matrix, matrix.inverse())
{}

template<typename T>
inline TTransform4<T>::TTransform4(const TMatrix4<T>& matrix, const TMatrix4<T>& inverseMatrix) :
    _matrix(matrix),
    _inverseMatrix(inverseMatrix)
{}

template<typename T>
inline TTransform4<T>::TTransform4(const TTransform4<T>& other) = default;

template<typename T>
inline TTransform4<T> TTransform4<T>::inverse() const
{
    return TTransform4<T>(_inverseMatrix, _matrix);
}

template<typename T>
inline void TTransform4<T>::transformPoint(
    const TVector3<T>& point,
    TVector3<T>* const out_point) const
{
    CS_ASSERT(out_point);

    _matrix.transformPoint(point, out_point);
}

template<typename T>
inline void TTransform4<T>::transformVector(
    const TVector3<T>& vector,
    TVector3<T>* const out_vector) const
{
    CS_ASSERT(out_vector);

    _matrix.transformVector(vector, out_vector);
}

} // namespace cadise