#include "math/transform.h"

#include "fundamental/assertion.h"
#include "math/vector.h"

namespace cadise {

Transform::Transform(const Matrix4& matrix) :
    Transform(matrix, matrix.inverse()) {
}

Transform::Transform(const Matrix4& matrix, const Matrix4& inverseMatrix) :
    _matrix(matrix),
    _inverseMatrix(inverseMatrix) {
}

void Transform::transformPoint(const Vector3R& point, Vector3R* const out_point) const {
    CADISE_ASSERT(out_point);

    _matrix.transformPoint(point, out_point);
}

void Transform::transformVector(const Vector3R& vector, Vector3R* const out_vector) const {
    CADISE_ASSERT(out_vector);

    _matrix.transformVector(vector, out_vector);
}

const Matrix4& Transform::matrix() const {
    return _matrix;
}

const Matrix4& Transform::inverseMatrix() const {
    return _inverseMatrix;
}

} // namespace cadise