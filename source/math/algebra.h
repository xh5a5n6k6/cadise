#pragma once

#include "core/basicHeader.h"

namespace cadise {

class Vector3 {
public:
	Vector3();
	Vector3(float x, float y, float z);

	Vector3 operator-();
	Vector3 operator+(const Vector3 &v);
	Vector3 operator-(const Vector3 &v);
	Vector3 operator*(const float s);
	Vector3 operator*(const Vector3 &v);
	Vector3 operator/(const float s);
	Vector3 operator/(const Vector3 &v);
	Vector3& operator+=(const Vector3 &v);
	Vector3& operator-=(const Vector3 &v);
	Vector3& operator*=(const float s);
	Vector3& operator*=(const Vector3 &v);
	Vector3& operator/=(const float s);
	Vector3& operator/=(const Vector3 &v);
	Vector3& operator=(const Vector3 &v);

	float length();
	float squaredLength();
	Vector3 normalize();
	Vector3 clamp(float min, float max);
	void swap(Vector3 &v);

	float x();
	float y();
	float z();

private:
	float _x, _y, _z;
};


class Matrix4 {
public:
	Matrix4();
	Matrix4(float n00, float n01, float n02, float n03,
			float n10, float n11, float n12, float n13,
			float n20, float n21, float n22, float n23,
			float n30, float n31, float n32, float n33);

	Matrix4& operator=(const Matrix4 &mat);

	Matrix4 transpose();
	Matrix4 inverse();

	float& n(int i, int j);

	// swap two rows
	void _swapRows(int r1, int r2);
	// divide row r with scalar s
	void _divideRow(int r, float s);
	// substract row r1 with row r2 multiplied by scalar s
	// i.e. r1 -= r2*s
	void _substractRow(int r1, int r2, float s);

	float _n[4][4];
};

inline Vector3 operator*(float s, Vector3 v) {
	return v * s;
}

inline float Dot(Vector3 v1, Vector3 v2) {
	return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}

inline float AbsDot(Vector3 v1, Vector3 v2) {
	return std::abs(v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z());
}

inline Vector3 Cross(Vector3 v1, Vector3 v2) {
	return Vector3(v1.y()*v2.z() - v1.z()*v2.y(),
				   v1.z()*v2.x() - v1.x()*v2.z(),
				   v1.x()*v2.y() - v1.y()*v2.x());
}

inline Vector3 TransformPoint(Matrix4 mat, Vector3 v) {
	return Vector3(mat._n[0][0] * v.x() + mat._n[0][1] * v.y() + mat._n[0][2] * v.z() + mat._n[0][3],
				   mat._n[1][0] * v.x() + mat._n[1][1] * v.y() + mat._n[1][2] * v.z() + mat._n[1][3],
				   mat._n[2][0] * v.x() + mat._n[2][1] * v.y() + mat._n[2][2] * v.z() + mat._n[2][3]);
}

inline Vector3 TransformVector(Matrix4 mat, Vector3 v) {
	return Vector3(mat._n[0][0] * v.x() + mat._n[0][1] * v.y() + mat._n[0][2] * v.z(),
				   mat._n[1][0] * v.x() + mat._n[1][1] * v.y() + mat._n[1][2] * v.z(),
				   mat._n[2][0] * v.x() + mat._n[2][1] * v.y() + mat._n[2][2] * v.z());
}

inline Matrix4 Matrix4Identity() {
	return Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
				   0.0f, 1.0f, 0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix4 Matrix4Scale(float sx, float sy, float sz) {
	return Matrix4(  sx, 0.0f, 0.0f, 0.0f,
				   0.0f,   sy, 0.0f, 0.0f,
				   0.0f, 0.0f,   sz, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix4 Matrix4Translate(float tx, float ty, float tz) {
	return Matrix4(1.0f, 0.0f, 0.0f,  tx,
				   0.0f, 1.0f, 0.0f,  ty,
				   0.0f, 0.0f, 1.0f,  tz,
				   0.0f, 0.0f, 0.0f, 1.0f);
}

// Return worldToCamera matrix
inline Matrix4 LookAt(Vector3 pos, Vector3 tar, Vector3 up) {
	Vector3 newZ = (pos - tar).normalize();
	Vector3 newX = Cross(up, newZ).normalize();
	Vector3 newY = Cross(newZ, newX);

	Matrix4 cameraToWorld = Matrix4(newX.x(), newY.x(), newZ.x(), pos.x(),
									newX.y(), newY.y(), newZ.y(), pos.y(),
									newX.z(), newY.z(), newZ.z(), pos.z(),
									    0.0f,     0.0f,     0.0f,    1.0f);

	return cameraToWorld.inverse();
}

} // namespace cadise