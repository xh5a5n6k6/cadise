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

    void transformPoint(
        const TVector3<T>& point,
        TVector3<T>* const out_point) const;

    void transformVector(
        const TVector3<T>& vector,
        TVector3<T>* const out_vector) const;

    const TMatrix4<T>& getMatrix() const;
    const TMatrix4<T>& getInverseMatrix() const;

private:
    TMatrix4<T> _matrix;
    TMatrix4<T> _inverseMatrix;
};

} // namespace cadise

#include "Math/TTransform4.ipp"