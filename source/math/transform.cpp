#include "math/transform.h"

#include "fundamental/assertion.h"
#include "math/tVector.h"

namespace cadise {

Transform::Transform(const Matrix4R& matrix) :
    Transform(matrix, matrix.inverse()) {
}

Transform::Transform(const Matrix4R& matrix, const Matrix4R& inverseMatrix) :
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

const Matrix4R& Transform::matrix() const {
    return _matrix;
}

const Matrix4R& Transform::inverseMatrix() const {
    return _inverseMatrix;
}

} // namespace cadise