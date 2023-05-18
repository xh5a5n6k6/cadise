#pragma once

#include "Math/TTransform.h"

#include "Foundation/Assertion.h"
#include "Math/TVector3.h"

namespace cadise
{

template<typename T>
inline TTransform<T>::TTransform(const TMatrix4<T>& matrix) :
    TTransform(matrix, matrix.inverse())
{}

template<typename T>
inline TTransform<T>::TTransform(const TMatrix4<T>& matrix, const TMatrix4<T>& inverseMatrix) :
    _matrix(matrix),
    _inverseMatrix(inverseMatrix)
{}

template<typename T>
inline void TTransform<T>::transformPoint(
    const TVector3<T>& point,
    TVector3<T>* const out_point) const
{
    CS_ASSERT(out_point);

    _matrix.transformPoint(point, out_point);
}

template<typename T>
inline void TTransform<T>::transformVector(
    const TVector3<T>& vector,
    TVector3<T>* const out_vector) const
{
    CS_ASSERT(out_vector);

    _matrix.transformVector(vector, out_vector);
}

template<typename T>
inline const TMatrix4<T>& TTransform<T>::getMatrix() const
{
    return _matrix;
}

template<typename T>
inline const TMatrix4<T>& TTransform<T>::getInverseMatrix() const
{
    return _inverseMatrix;
}

} // namespace cadise