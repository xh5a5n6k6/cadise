#pragma once

#include "math/matrix.h"

namespace cadise {

class Transform {
public:
    Transform(const Matrix4& matrix);
    Transform(const Matrix4& matrix, const Matrix4& inverseMatrix);

    Vector3R transformPoint(const Vector3R& point) const;
    Vector3R transformVector(const Vector3R& vector) const;

    const Matrix4& matrix() const;
    const Matrix4& inverseMatrix() const;

private:
    Matrix4 _matrix;
    Matrix4 _inverseMatrix;
};

} // namespace cadise