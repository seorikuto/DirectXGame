﻿#include "Matrix4x4.h"
#include <cmath>


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



 Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	 Matrix4x4 result;
	 result = {scale.x, 0.0f, 0.0f,    0.0f, 0.0f, scale.y, 0.0f, 0.0f,
	           0.0f,    0.0f, scale.z, 0.0f, 0.0f, 0.0f,    0.0f, 1.0f};
	 return result;
 }

Matrix4x4 MakeRotateXMatrix(float theta) {
	 Matrix4x4 result;
	 float sin = std::sin(theta);
	 float cos = std::cos(theta);

	 result = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos,  sin,  0.0f,
	           0.0f, -sin, cos,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	 return result;
}

Matrix4x4 MakeRotateYMatrix(float theta) {

	 Matrix4x4 result;
	 float sin = std::sin(theta);
	 float cos = std::cos(theta);

	 result = {cos, 0.0f, -sin, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	           sin, 0.0f, cos,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	 return result;
}


Matrix4x4 MakeRotateZMatrix(float theta) {

	 Matrix4x4 result;
	 float sin = std::sin(theta);
	 float cos = std::cos(theta);

	 result = {cos,  sin,  0.0f, 0.0f, -sin, cos,  0.0f, 0.0f,
	           0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	 return result;
}

Matrix4x4 MakeTranslateMatrix(Vector3 translate) {
	 Matrix4x4 result;
	 result = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f,        1.0f,        0.0f,        0.0f,
	           0.0f, 0.0f, 1.0f, 0.0f, translate.x, translate.y, translate.z, 1.0f};
	 return result;
}



Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {
	 Matrix4x4 matScale = MakeScaleMatrix(scale);

	 Matrix4x4 matRotX = MakeRotateXMatrix(rot.x);
	 Matrix4x4 matRotY = MakeRotateYMatrix(rot.y);
	 Matrix4x4 matRotZ = MakeRotateZMatrix(rot.z);

	 Matrix4x4 matRot = matRotX * matRotY * matRotZ;

	 Matrix4x4 matTrans = MakeTranslateMatrix(translate);

	 Matrix4x4 result = matScale * matRot * matTrans;

	 return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	 Vector3 result
	 {v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
	  v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[1][2],
	  v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]
	 };
	 return result;
}

Matrix4x4 Inverse(Matrix4x4 matrix) {
	 Matrix4x4 result;
	 float determinant = matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
	                     matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
	                     matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
	                     matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
	                     matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
	                     matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2] -
	                     matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
	                     matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1] -
	                     matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] +
	                     matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1] +
	                     matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
	                     matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2] +
	                     matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
	                     matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] +
	                     matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] -
	                     matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] -
	                     matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
	                     matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] -
	                     matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
	                     matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
	                     matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] +
	                     matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
	                     matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
	                     matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0];

	 float determinantRecp = 1.0f / determinant;

	 result.m[0][0] = (matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][3] +
	                   matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][1] +
	                   matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][2] -
	                   matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][1] -
	                   matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][3] -
	                   matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][2]) *
	                  determinantRecp;
	 result.m[0][1] = (-matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][3] -
	                   matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][1] -
	                   matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][2] +
	                   matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][1] +
	                   matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][3] +
	                   matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][2]) *
	                  determinantRecp;
	 result.m[0][2] = (matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][3] +
	                   matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][1] +
	                   matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][2] -
	                   matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][1] -
	                   matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][3] -
	                   matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][2]) *
	                  determinantRecp;
	 result.m[0][3] = (-matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][3] -
	                   matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][1] -
	                   matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][2] +
	                   matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][1] +
	                   matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][3] +
	                   matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][2]) *
	                  determinantRecp;
	 result.m[1][0] = (-matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][3] -
	                   matrix.m[1][2] * matrix.m[2][3] * matrix.m[3][0] -
	                   matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][2] +
	                   matrix.m[1][3] * matrix.m[2][2] * matrix.m[3][0] +
	                   matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][3] +
	                   matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][2]) *
	                  determinantRecp;
	 result.m[1][1] = (matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][3] +
	                   matrix.m[0][2] * matrix.m[2][3] * matrix.m[3][0] +
	                   matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][2] -
	                   matrix.m[0][3] * matrix.m[2][2] * matrix.m[3][0] -
	                   matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][3] -
	                   matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][2]) *
	                  determinantRecp;
	 result.m[1][2] = (-matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][3] -
	                   matrix.m[0][2] * matrix.m[1][3] * matrix.m[3][0] -
	                   matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][2] +
	                   matrix.m[0][3] * matrix.m[1][2] * matrix.m[3][0] +
	                   matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][3] +
	                   matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][2]) *
	                  determinantRecp;
	 result.m[1][3] = (matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][3] +
	                   matrix.m[0][2] * matrix.m[1][3] * matrix.m[2][0] +
	                   matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][2] -
	                   matrix.m[0][3] * matrix.m[1][2] * matrix.m[2][0] -
	                   matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][3] -
	                   matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][2]) *
	                  determinantRecp;
	 result.m[2][0] = (matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][3] +
	                   matrix.m[1][1] * matrix.m[2][3] * matrix.m[3][0] +
	                   matrix.m[1][3] * matrix.m[2][0] * matrix.m[3][1] -
	                   matrix.m[1][3] * matrix.m[2][1] * matrix.m[3][0] -
	                   matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][3] -
	                   matrix.m[1][0] * matrix.m[2][3] * matrix.m[3][1]) *
	                  determinantRecp;
	 result.m[2][1] = (-matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][3] -
	                   matrix.m[0][1] * matrix.m[2][3] * matrix.m[3][0] -
	                   matrix.m[0][3] * matrix.m[2][0] * matrix.m[3][1] +
	                   matrix.m[0][3] * matrix.m[2][1] * matrix.m[3][0] +
	                   matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][3] +
	                   matrix.m[0][0] * matrix.m[2][3] * matrix.m[3][1]) *
	                  determinantRecp;
	 result.m[2][2] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][3] +
	                   matrix.m[0][1] * matrix.m[1][3] * matrix.m[3][0] +
	                   matrix.m[0][3] * matrix.m[1][0] * matrix.m[3][1] -
	                   matrix.m[0][3] * matrix.m[1][1] * matrix.m[3][0] -
	                   matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][3] -
	                   matrix.m[0][0] * matrix.m[1][3] * matrix.m[3][1]) *
	                  determinantRecp;
	 result.m[2][3] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][3] -
	                   matrix.m[0][1] * matrix.m[1][3] * matrix.m[2][0] -
	                   matrix.m[0][3] * matrix.m[1][0] * matrix.m[2][1] +
	                   matrix.m[0][3] * matrix.m[1][1] * matrix.m[2][0] +
	                   matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][3] +
	                   matrix.m[0][0] * matrix.m[1][3] * matrix.m[2][1]) *
	                  determinantRecp;
	 result.m[3][0] = (-matrix.m[1][0] * matrix.m[2][1] * matrix.m[3][2] -
	                   matrix.m[1][1] * matrix.m[2][2] * matrix.m[3][0] -
	                   matrix.m[1][2] * matrix.m[2][0] * matrix.m[3][1] +
	                   matrix.m[1][2] * matrix.m[2][1] * matrix.m[3][0] +
	                   matrix.m[1][1] * matrix.m[2][0] * matrix.m[3][2] +
	                   matrix.m[1][0] * matrix.m[2][2] * matrix.m[3][1]) *
	                  determinantRecp;
	 result.m[3][1] = (matrix.m[0][0] * matrix.m[2][1] * matrix.m[3][2] +
	                   matrix.m[0][1] * matrix.m[2][2] * matrix.m[3][0] +
	                   matrix.m[0][2] * matrix.m[2][0] * matrix.m[3][1] -
	                   matrix.m[0][2] * matrix.m[2][1] * matrix.m[3][0] -
	                   matrix.m[0][1] * matrix.m[2][0] * matrix.m[3][2] -
	                   matrix.m[0][0] * matrix.m[2][2] * matrix.m[3][1]) *
	                  determinantRecp;
	 result.m[3][2] = (-matrix.m[0][0] * matrix.m[1][1] * matrix.m[3][2] -
	                   matrix.m[0][1] * matrix.m[1][2] * matrix.m[3][0] -
	                   matrix.m[0][2] * matrix.m[1][0] * matrix.m[3][1] +
	                   matrix.m[0][2] * matrix.m[1][1] * matrix.m[3][0] +
	                   matrix.m[0][1] * matrix.m[1][0] * matrix.m[3][2] +
	                   matrix.m[0][0] * matrix.m[1][2] * matrix.m[3][1]) *
	                  determinantRecp;
	 result.m[3][3] = (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
	                   matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
	                   matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
	                   matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
	                   matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
	                   matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]) *
	                  determinantRecp;
	 return result;
}

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
	 Matrix4x4 result;
	 result.m[0][0] = width / 2.0f;
	 result.m[0][1] = 0.0f;
	 result.m[0][2] = 0.0f;
	 result.m[0][3] = 0.0f;
	 result.m[1][0] = 0.0f;
	 result.m[1][1] = -height / 2.0f;
	 result.m[1][2] = 0.0f;
	 result.m[1][3] = 0.0f;
	 result.m[2][0] = 0.0f;
	 result.m[2][1] = 0.0f;
	 result.m[2][2] = (maxDepth - minDepth);
	 result.m[2][3] = 0.0f;
	 result.m[3][0] = left + (width / 2);
	 result.m[3][1] = top + (height / 2);
	 result.m[3][2] = minDepth;
	 result.m[3][3] = 1.0f;
	 return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	 Vector3 result;
	 result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] +
	            matrix.m[3][0];
	 result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] +
	            matrix.m[3][1];
	 result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] +
	            matrix.m[3][2];
	 float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] +
	           matrix.m[3][3];

	 // assert(w != 0.0f);
	 result.x /= w;
	 result.y /= w;
	 result.z /= w;
	 return result;
}

Vector3 Normalize(const Vector3& v) {
	 Vector3 result;
	 result.x = v.x / sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	 result.y = v.y / sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	 result.z = v.z / sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	 return result;
}