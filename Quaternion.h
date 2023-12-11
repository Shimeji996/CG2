#pragma once
#include "Matrix.h"

struct Quaternion
{
	float num[4];
};

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
Quaternion IdentityQuaternion();
Quaternion Conjugate(const Quaternion& quaternion);
float Norm(const Quaternion& quaternion);
Quaternion Normalize(const Quaternion& quaternion);
Quaternion Inverse(const Quaternion& quaternion);
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);
