﻿#include "Matrix.h"

Vector3 Normalise(const Vector3& v)
{
	float len = Length(v);
	if (len != 0)
	{
		return { v.num[0] / len,v.num[1] / len,v.num[2] / len };
	}
	return v;
}

float Length(const Vector3& v)
{
	return sqrtf(Dot(v, v));
}

float Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.num[0] * v2.num[0] + v1.num[1] * v2.num[1] + v1.num[2] * v2.num[2];
}

//X軸回転行列
Matrix4x4 MakeRotateXmatrix(float radian)
{
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -std::sin(radian);
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

//Y軸回転行列
Matrix4x4 MakeRotateYmatrix(float radian)
{
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -std::sin(radian);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

//Z軸回転行列
Matrix4x4 MakeRotateZmatrix(float radian)
{
	Matrix4x4 result;
	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -std::sin(radian);
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

//平行移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate)
{
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.num[0];
	result.m[3][1] = translate.num[1];
	result.m[3][2] = translate.num[2];
	result.m[3][3] = 1.0f;

	return result;
};

//拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result;

	result.m[0][0] = scale.num[0];
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.num[1];
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.num[2];
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

//アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 result;
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXmatrix(rotate.num[0]);
	Matrix4x4 rotateYMatrix = MakeRotateYmatrix(rotate.num[1]);
	Matrix4x4 rotateZMatrix = MakeRotateZmatrix(rotate.num[2]);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	result = Multiply(scaleMatrix, Multiply(rotateXYZMatrix, translateMatrix));
	return result;
}

//行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;
}

//行列の減法
Matrix4x4 Sub(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}

//行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float Term = 0;
			for (int k = 0; k < 4; k++)
			{
				Term = Term + m1.m[i][k] * m2.m[k][j];
				result.m[i][j] = Term;
			}
		}
	}
	return result;
}

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m1)
{
	Matrix4x4 result;
	float deterninant = m1.m[0][0] * m1.m[1][1] * m1.m[2][2] * m1.m[3][3] +
		m1.m[0][0] * m1.m[1][2] * m1.m[2][3] * m1.m[3][1] +
		m1.m[0][0] * m1.m[1][3] * m1.m[2][1] * m1.m[3][2] -

		m1.m[0][0] * m1.m[1][3] * m1.m[2][2] * m1.m[3][1] -
		m1.m[0][0] * m1.m[1][2] * m1.m[2][1] * m1.m[3][3] -
		m1.m[0][0] * m1.m[1][1] * m1.m[2][3] * m1.m[3][2] -

		m1.m[0][1] * m1.m[1][0] * m1.m[2][2] * m1.m[3][3] -
		m1.m[0][2] * m1.m[1][0] * m1.m[2][3] * m1.m[3][1] -
		m1.m[0][3] * m1.m[1][0] * m1.m[2][1] * m1.m[3][2] +

		m1.m[0][3] * m1.m[1][0] * m1.m[2][2] * m1.m[3][1] +
		m1.m[0][2] * m1.m[1][0] * m1.m[2][1] * m1.m[3][3] +
		m1.m[0][1] * m1.m[1][0] * m1.m[2][3] * m1.m[3][2] +

		m1.m[0][1] * m1.m[1][2] * m1.m[2][0] * m1.m[3][3] +
		m1.m[0][2] * m1.m[1][3] * m1.m[2][0] * m1.m[3][1] +
		m1.m[0][3] * m1.m[1][1] * m1.m[2][0] * m1.m[3][2] -

		m1.m[0][3] * m1.m[1][2] * m1.m[2][0] * m1.m[3][1] -
		m1.m[0][2] * m1.m[1][1] * m1.m[2][0] * m1.m[3][3] -
		m1.m[0][1] * m1.m[1][3] * m1.m[2][0] * m1.m[3][2] -

		m1.m[0][1] * m1.m[1][2] * m1.m[2][3] * m1.m[3][0] -
		m1.m[0][2] * m1.m[1][3] * m1.m[2][1] * m1.m[3][0] -
		m1.m[0][3] * m1.m[1][1] * m1.m[2][2] * m1.m[3][0] +

		m1.m[0][3] * m1.m[1][2] * m1.m[2][1] * m1.m[3][0] +
		m1.m[0][2] * m1.m[1][1] * m1.m[2][3] * m1.m[3][0] +
		m1.m[0][1] * m1.m[1][3] * m1.m[2][2] * m1.m[3][0];

	assert(deterninant != 0.0f);
	float deterninantRect = 1.0f / deterninant;

	result.m[0][0] =
		(m1.m[1][1] * m1.m[2][2] * m1.m[3][3] +
			m1.m[1][2] * m1.m[2][3] * m1.m[3][1] +
			m1.m[1][3] * m1.m[2][1] * m1.m[3][2] -
			m1.m[1][3] * m1.m[2][2] * m1.m[3][1] -
			m1.m[1][2] * m1.m[2][1] * m1.m[3][3] -
			m1.m[1][1] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[0][1] =
		(-m1.m[0][1] * m1.m[2][2] * m1.m[3][3] -
			m1.m[0][2] * m1.m[2][3] * m1.m[3][1] -
			m1.m[0][3] * m1.m[2][1] * m1.m[3][2] +
			m1.m[0][3] * m1.m[2][2] * m1.m[3][1] +
			m1.m[0][2] * m1.m[2][1] * m1.m[3][3] +
			m1.m[0][1] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[0][2] =
		(m1.m[0][1] * m1.m[1][2] * m1.m[3][3] +
			m1.m[0][2] * m1.m[1][3] * m1.m[3][1] +
			m1.m[0][3] * m1.m[1][1] * m1.m[3][2] -
			m1.m[0][3] * m1.m[1][2] * m1.m[3][1] -
			m1.m[0][2] * m1.m[1][1] * m1.m[3][3] -
			m1.m[0][1] * m1.m[1][3] * m1.m[3][2]) * deterninantRect;
	result.m[0][3] =
		(-m1.m[0][1] * m1.m[1][2] * m1.m[2][3] -
			m1.m[0][2] * m1.m[1][3] * m1.m[2][1] -
			m1.m[0][3] * m1.m[1][1] * m1.m[2][2] +
			m1.m[0][3] * m1.m[1][2] * m1.m[2][1] +
			m1.m[0][2] * m1.m[1][1] * m1.m[2][3] +
			m1.m[0][1] * m1.m[1][3] * m1.m[2][2]) * deterninantRect;

	result.m[1][0] =
		(-m1.m[1][0] * m1.m[2][2] * m1.m[3][3] -
			m1.m[1][2] * m1.m[2][3] * m1.m[3][0] -
			m1.m[1][3] * m1.m[2][0] * m1.m[3][2] +
			m1.m[1][3] * m1.m[2][2] * m1.m[3][0] +
			m1.m[1][2] * m1.m[2][0] * m1.m[3][3] +
			m1.m[1][0] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[1][1] =
		(m1.m[0][0] * m1.m[2][2] * m1.m[3][3] +
			m1.m[0][2] * m1.m[2][3] * m1.m[3][0] +
			m1.m[0][3] * m1.m[2][0] * m1.m[3][2] -
			m1.m[0][3] * m1.m[2][2] * m1.m[3][0] -
			m1.m[0][2] * m1.m[2][0] * m1.m[3][3] -
			m1.m[0][0] * m1.m[2][3] * m1.m[3][2]) * deterninantRect;
	result.m[1][2] =
		(-m1.m[0][0] * m1.m[1][2] * m1.m[3][3] -
			m1.m[0][2] * m1.m[1][3] * m1.m[3][0] -
			m1.m[0][3] * m1.m[1][0] * m1.m[3][2] +
			m1.m[0][3] * m1.m[1][2] * m1.m[3][0] +
			m1.m[0][2] * m1.m[1][0] * m1.m[3][3] +
			m1.m[0][0] * m1.m[1][3] * m1.m[3][2]) * deterninantRect;
	result.m[1][3] =
		(m1.m[0][0] * m1.m[1][2] * m1.m[2][3] +
			m1.m[0][2] * m1.m[1][3] * m1.m[2][0] +
			m1.m[0][3] * m1.m[1][0] * m1.m[2][2] -
			m1.m[0][3] * m1.m[1][2] * m1.m[2][0] -
			m1.m[0][2] * m1.m[1][0] * m1.m[2][3] -
			m1.m[0][0] * m1.m[1][3] * m1.m[2][2]) * deterninantRect;

	result.m[2][0] =
		(m1.m[1][0] * m1.m[2][1] * m1.m[3][3] +
			m1.m[1][1] * m1.m[2][3] * m1.m[3][0] +
			m1.m[1][3] * m1.m[2][0] * m1.m[3][1] -
			m1.m[1][3] * m1.m[2][1] * m1.m[3][0] -
			m1.m[1][1] * m1.m[2][0] * m1.m[3][3] -
			m1.m[1][0] * m1.m[2][3] * m1.m[3][1]) * deterninantRect;
	result.m[2][1] =
		(-m1.m[0][0] * m1.m[2][1] * m1.m[3][3] -
			m1.m[0][1] * m1.m[2][3] * m1.m[3][0] -
			m1.m[0][3] * m1.m[2][0] * m1.m[3][1] +
			m1.m[0][3] * m1.m[2][1] * m1.m[3][0] +
			m1.m[0][1] * m1.m[2][0] * m1.m[3][3] +
			m1.m[0][0] * m1.m[2][3] * m1.m[3][1]) * deterninantRect;
	result.m[2][2] =
		(m1.m[0][0] * m1.m[1][1] * m1.m[3][3] +
			m1.m[0][1] * m1.m[1][3] * m1.m[3][0] +
			m1.m[0][3] * m1.m[1][0] * m1.m[3][1] -
			m1.m[0][3] * m1.m[1][1] * m1.m[3][0] -
			m1.m[0][1] * m1.m[1][0] * m1.m[3][3] -
			m1.m[0][0] * m1.m[1][3] * m1.m[3][1]) * deterninantRect;
	result.m[2][3] =
		(-m1.m[0][0] * m1.m[1][1] * m1.m[2][3] -
			m1.m[0][1] * m1.m[1][3] * m1.m[2][0] -
			m1.m[0][3] * m1.m[1][0] * m1.m[2][1] +
			m1.m[0][3] * m1.m[1][1] * m1.m[2][0] +
			m1.m[0][1] * m1.m[1][0] * m1.m[2][3] +
			m1.m[0][0] * m1.m[1][3] * m1.m[2][1]) * deterninantRect;

	result.m[3][0] =
		(-m1.m[1][0] * m1.m[2][1] * m1.m[3][2] -
			m1.m[1][1] * m1.m[2][2] * m1.m[3][0] -
			m1.m[1][2] * m1.m[2][0] * m1.m[3][1] +
			m1.m[1][2] * m1.m[2][1] * m1.m[3][0] +
			m1.m[1][1] * m1.m[2][0] * m1.m[3][2] +
			m1.m[1][0] * m1.m[2][2] * m1.m[3][1]) * deterninantRect;
	result.m[3][1] =
		(m1.m[0][0] * m1.m[2][1] * m1.m[3][2] +
			m1.m[0][1] * m1.m[2][2] * m1.m[3][0] +
			m1.m[0][2] * m1.m[2][0] * m1.m[3][1] -
			m1.m[0][2] * m1.m[2][1] * m1.m[3][0] -
			m1.m[0][1] * m1.m[2][0] * m1.m[3][2] -
			m1.m[0][0] * m1.m[2][2] * m1.m[3][1]) * deterninantRect;
	result.m[3][2] =
		(-m1.m[0][0] * m1.m[1][1] * m1.m[3][2] -
			m1.m[0][1] * m1.m[1][2] * m1.m[3][0] -
			m1.m[0][2] * m1.m[1][0] * m1.m[3][1] +
			m1.m[0][2] * m1.m[1][1] * m1.m[3][0] +
			m1.m[0][1] * m1.m[1][0] * m1.m[3][2] +
			m1.m[0][0] * m1.m[1][2] * m1.m[3][1]) * deterninantRect;
	result.m[3][3] =
		(m1.m[0][0] * m1.m[1][1] * m1.m[2][2] +
			m1.m[0][1] * m1.m[1][2] * m1.m[2][0] +
			m1.m[0][2] * m1.m[1][0] * m1.m[2][1] -
			m1.m[0][2] * m1.m[1][1] * m1.m[2][0] -
			m1.m[0][1] * m1.m[1][0] * m1.m[2][2] -
			m1.m[0][0] * m1.m[1][2] * m1.m[2][1]) * deterninantRect;

	return result;
}

//転置行列
Matrix4x4 Transpose(const Matrix4x4& m)
{
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[j][i] = m.m[i][j];
		}
	}
	return result;
}

//単位行列
Matrix4x4 MakeIdentity4x4()
{
	return{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

float contangent(float b, float a)
{
	return(b / tan(a));
}

//透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRadio, float nearClip, float farClip)
{
	Matrix4x4 result;
	result.m[0][0] = contangent((1 / aspectRadio), (fovY / 2));
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = contangent(1, fovY / 2);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = -nearClip * farClip / (farClip - nearClip);
	result.m[3][3] = 0.0f;

	return result;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result;
	result.m[0][0] = 2 / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;
	result.m[1][0] = 0;
	result.m[1][1] = 2 / (top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;
	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[2][3] = 0;
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;
	return result;
}

//任意軸回転行列
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Matrix4x4 result;

	result.m[0][0] = (axis.num[0] * axis.num[0]) * (1 - cos(angle)) + cos(angle);
	result.m[0][1] = (axis.num[0] * axis.num[1]) * (1 - cos(angle)) + axis.num[2] * sin(angle);
	result.m[0][2] = (axis.num[0] * axis.num[2]) * (1 - cos(angle)) - axis.num[1] * sin(angle);
	result.m[0][3] = 0;

	result.m[1][0] = (axis.num[0] * axis.num[1]) * (1 - cos(angle)) - axis.num[2] * sin(angle);
	result.m[1][1] = (axis.num[1] * axis.num[1]) * (1 - cos(angle)) + cos(angle);
	result.m[1][2] = (axis.num[1] * axis.num[2]) * (1 - cos(angle)) + axis.num[0] * sin(angle);
	result.m[1][3] = 0;

	result.m[2][0] = (axis.num[0] * axis.num[2]) * (1 - cos(angle)) + axis.num[1] * sin(angle);
	result.m[2][1] = (axis.num[1] * axis.num[2]) * (1 - cos(angle)) - axis.num[0] * sin(angle);
	result.m[2][2] = (axis.num[2] * axis.num[2]) * (1 - cos(angle)) + cos(angle);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.num[0] = (v1.num[1] * v2.num[2]) - (v1.num[2] * v2.num[1]);
	result.num[1] = (v1.num[2] * v2.num[0]) - (v1.num[0] * v2.num[2]);
	result.num[2] = (v1.num[0] * v2.num[1]) - (v1.num[1] * v2.num[0]);
	return result;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Matrix4x4 result;
	Vector3 cross = Cross(from, to);
	Vector3 n = Normalise(Cross(from, to));
	// u = -v のとき　つまり反転してしまった時
	if (from.num[0] == -to.num[0] && from.num[1] == -to.num[1] && from.num[2] == -to.num[2]) {
		if (from.num[0] != 0.0f || from.num[1] != 0.0f) {
			n = { from.num[1],-from.num[0],0.0f };
		}
		else if (from.num[0] != 0.0f || from.num[2] != 0.0f) {
			n = { from.num[2],0.0f,-from.num[0] };
		}
	}

	float costhata = Dot(from, to);
	float sinthata = Length(cross);
	result.m[0][0] = (n.num[0] * n.num[0]) * (1 - costhata) + costhata;
	result.m[0][1] = (n.num[0] * n.num[1]) * (1 - costhata) + n.num[2] * sinthata;
	result.m[0][2] = (n.num[0] * n.num[2]) * (1 - costhata) - n.num[1] * sinthata;
	result.m[0][3] = 0;

	result.m[1][0] = (n.num[0] * n.num[1]) * (1 - costhata) - n.num[2] * sinthata;
	result.m[1][1] = (n.num[1] * n.num[1]) * (1 - costhata) + costhata;
	result.m[1][2] = (n.num[1] * n.num[2]) * (1 - costhata) + n.num[0] * sinthata;
	result.m[1][3] = 0;

	result.m[2][0] = (n.num[0] * n.num[2]) * (1 - costhata) + n.num[1] * sinthata;
	result.m[2][1] = (n.num[1] * n.num[2]) * (1 - costhata) - n.num[0] * sinthata;
	result.m[2][2] = (n.num[2] * n.num[2]) * (1 - costhata) + costhata;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Vector3 VectorTransform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;
	result.num[0] = vector.num[0] * matrix.m[0][0] + vector.num[1] * matrix.m[1][0] + vector.num[2] * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.num[1] = vector.num[0] * matrix.m[0][1] + vector.num[1] * matrix.m[1][1] + vector.num[2] * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.num[2] = vector.num[0] * matrix.m[0][2] + vector.num[1] * matrix.m[1][2] + vector.num[2] * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.num[0] * matrix.m[0][3] + vector.num[1] * matrix.m[1][3] + vector.num[2] * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	result.num[0] /= w;
	result.num[1] /= w;
	result.num[2] /= w;
	return result;
}