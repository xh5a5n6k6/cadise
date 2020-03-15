#pragma once

#include "math/matrix.h"

namespace cadise {

class Transform {
public:
    explicit Transform(const Matrix4& matrix);
    Transform(const Matrix4& matrix, const Matrix4& inverseMatrix);

    void transformPoint(const Vector3R& point, Vector3R* const out_point) const;
    void transformVector(const Vector3R& vector, Vector3R* const out_vector) const;

    const Matrix4& matrix() const;
    const Matrix4& inverseMatrix() const;

private:
    Matrix4 _matrix;
    Matrix4 _inverseMatrix;
};

} // namespace cadise