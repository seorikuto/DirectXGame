#pragma once
#include "Vector3.h"

    /// <summary>
    /// 4x4行列
    /// </summary>
    struct Matrix4x4 final {
	float m[4][4];
};

Matrix4x4 MakeScaleMatrix(const Vector3& scale);
Matrix4x4 MakeRotateXMatrix(float theta);
Matrix4x4 MakeRotateYMatrix(float theta);
Matrix4x4 MakeRotateZMatrix(float theta);
Matrix4x4 MakeTranslateMatrix(Vector3 translate);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = {};
	for (size_t i = 0; i < 4; i++) {
		for (rsize_t j = 0; j < 4; j++) {
			for (rsize_t k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	m1 = result;
	return m1;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;

	return result *= m2;
}