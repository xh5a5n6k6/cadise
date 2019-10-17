#include "math/transform.h"

#include "math/vector.h"

namespace cadise {

Transform::Transform(const Matrix4& matrix) :
    Transform(matrix, matrix.inverse()) {
}

Transform::Transform(const Matrix4& matrix, const Matrix4& inverseMatrix) :
    _matrix(matrix),
    _inverseMatrix(inverseMatrix) {
}

Vector3R Transform::transformPoint(const Vector3R& point) const {
    return _matrix.transformPoint(point);
}

Vector3R Transform::transformVector(const Vector3R& vector) const {
    return _matrix.transformVector(vector);
}

Matrix4 Transform::matrix() const {
    return _matrix;
}

Matrix4 Transform::inverseMatrix() const {
    return _inverseMatrix;
}

} // namespace cadise