﻿#pragma once
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
//ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

//逆行列
Matrix4x4 Inverse(Matrix4x4 matrix);

//ビューポート行列
Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);


 //代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);
//2公演算子オーバーロード 
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

