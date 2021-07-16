#pragma once

#include "math/tMatrix4.h"

namespace cadise {

class Transform {
public:
    explicit Transform(const Matrix4R& matrix);
    Transform(const Matrix4R& matrix, const Matrix4R& inverseMatrix);

    void transformPoint(const Vector3R& point, Vector3R* const out_point) const;
    void transformVector(const Vector3R& vector, Vector3R* const out_vector) const;

    const Matrix4R& matrix() const;
    const Matrix4R& inverseMatrix() const;

private:
    Matrix4R _matrix;
    Matrix4R _inverseMatrix;
};

} // namespace cadise