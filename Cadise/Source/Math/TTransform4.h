#pragma once

#include "Math/TMatrix4.h"
#include "Math/Type/MathType.h"

namespace cadise
{

template<typename T>
class TTransform4
{
public:
    TTransform4();
    explicit TTransform4(const TMatrix4<T>& matrix);
    TTransform4(const TMatrix4<T>& matrix, const TMatrix4<T>& inverseMatrix);
    TTransform4(const TTransform4& other);

    TTransform4 inverse() const;

    void transformPoint(
        const TVector3<T>& point,
        TVector3<T>* const out_point) const;

    void transformVector(
        const TVector3<T>& vector,
        TVector3<T>* const out_vector) const;

private:
    TMatrix4<T> _matrix;
    TMatrix4<T> _inverseMatrix;
};

} // namespace cadise

#include "Math/TTransform4.ipp"