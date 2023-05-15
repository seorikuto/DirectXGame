#pragma once
#include "Vector3.h"

/// <summary>
/// 4x4�s��
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

// ������Z�q�I�[�o�[���[�h
//Matrix4x4& operator=(Matrix4x4& m1, const Matrix4x4& m2);

// 2�����Z�q�I�[�o�[���[�h
//Matrix4x4 operator(const Matrix4x4& m1, const Matrix4x4& m2);